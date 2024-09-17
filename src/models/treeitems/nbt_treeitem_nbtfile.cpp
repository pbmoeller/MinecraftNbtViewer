// MinecraftNbtViewer
#include "nbt_treeitem_nbtfile.hpp"
#include "nbt_treeitem_nbttag.hpp"
#include "treeitem_util.hpp"

// cpp-anvil
#include <cpp-anvil/nbt/io.hpp>

namespace minecraft {
namespace nbt {

NbtTreeItemNbtFile::NbtTreeItemNbtFile(NbtTreeItemBase *parentItem,
                                       const QString &filename,
                                       const QString &pathToFile)
    : NbtTreeItemBase(parentItem)
    , m_canFetchData(true)
    , m_filename(filename)
    , m_pathToFile(pathToFile)
    , m_compressionType(anvil::CompressionType::Gzip)
{

}

NbtTreeItemNbtFile::~NbtTreeItemNbtFile() = default;

QIcon NbtTreeItemNbtFile::icon() const
{
    return QIcon(":/icons/16x16/NbtFile.png");
}

QString NbtTreeItemNbtFile::label() const
{
    QString label = m_filename;
    if(!m_canFetchData) {
        switch(m_compressionType) {
            case anvil::CompressionType::Gzip:
                label += " (Gzip)";
                break;
            case anvil::CompressionType::Zlib:
                label += " (Zlib)";
                break;
            case anvil::CompressionType::Uncompressed:
                label += " (No compression)";
                break;
        }
    }

    return label;
}

bool NbtTreeItemNbtFile::canSave() const
{
    return true;
}

void NbtTreeItemNbtFile::save()
{
    // Fetch data first if not already done or the root tag is empty.
    if(canFetchMore()) {
        fetchMore();
    }
    std::string filename = (m_pathToFile + "/" + m_filename).toStdString();
    anvil::saveToFile(filename, m_nbtRootTag.get(), m_compressionType);
}

void NbtTreeItemNbtFile::saveAs(const QString &filename, const anvil::CompressionType compression)
{
    // Fetch data first if not already done or the root tag is empty.
    if(canFetchMore()) {
        fetchMore();
    }
    anvil::saveToFile(filename.toStdString(), m_nbtRootTag.get(), compression);
}

bool NbtTreeItemNbtFile::canRefresh() const
{
    return true;
}

bool NbtTreeItemNbtFile::canFetchMore() const
{
    return m_canFetchData;
}

bool NbtTreeItemNbtFile::canOpenInExplorer() const
{
    return true;
}

QString NbtTreeItemNbtFile::path() const
{
    return m_pathToFile;
}

void NbtTreeItemNbtFile::fetchMore()
{
    m_canFetchData = false;

    // Read NBT data
    std::string filename = (m_pathToFile + "/" + m_filename).toStdString();
    m_nbtRootTag = anvil::loadFromFile(filename, m_compressionType);
    if(m_nbtRootTag) {
        anvil::CompoundTag *tag = anvil::tag_cast<anvil::CompoundTag*>(m_nbtRootTag.get());
        addNbtChild(this, tag);
        sort();
    }
}

NbtTreeItemBase* NbtTreeItemNbtFile::markItemDirty()
{
    return this;
}

NbtTreeItemNbtFile* NbtTreeItemNbtFile::createNewNbtFile(NbtTreeItemBase *parentItem,
                                                         const QString &pathToFile)
{
    if(!parentItem) {
        return nullptr;
    }

    NbtTreeItemNbtFile *newItem = new NbtTreeItemNbtFile(parentItem, QString(), pathToFile);
    newItem->m_canFetchData = false;
    newItem->m_compressionType = anvil::CompressionType::Uncompressed;
    newItem->m_nbtRootTag = std::make_unique<anvil::CompoundTag>();
    addNbtChild(newItem, newItem->m_nbtRootTag.get());

    return newItem;
}

anvil::CompressionType NbtTreeItemNbtFile::getCompression() const
{
    return m_compressionType;
}

} // namespace nbt
} // namespace minecraft
