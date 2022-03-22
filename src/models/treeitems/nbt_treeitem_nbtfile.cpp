// AwesomeNbtViewer
#include "nbt_treeitem_nbtfile.hpp"
#include "nbt_treeitem_nbttag.hpp"
#include "treeitem_util.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/nbt_read.hpp>
#include <AwesomeMC/nbt/nbt_write.hpp>

namespace anv
{

NbtTreeItemNbtFile::NbtTreeItemNbtFile(NbtTreeItemBase *parentItem,
                                       const QString &filename,
                                       const QString &pathToFile)
    : NbtTreeItemBase(parentItem)
    , m_canFetchData(true)
    , m_filename(filename)
    , m_pathToFile(pathToFile)
{

}

NbtTreeItemNbtFile::~NbtTreeItemNbtFile()
{

}

QIcon NbtTreeItemNbtFile::getIcon() const
{
    return QIcon(":/icons/16x16/NbtFile.png");
}

QString NbtTreeItemNbtFile::getLabel() const
{
    return m_filename;
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
    amc::writeNbtFile(filename, m_nbtRootTag.get(), true);
}

void NbtTreeItemNbtFile::saveAs(const QString &filename)
{
    // Fetch data first if not already done or the root tag is empty.
    if(canFetchMore()) {
        fetchMore();
    }
    amc::writeNbtFile(filename.toStdString(), m_nbtRootTag.get(), true);
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

QString NbtTreeItemNbtFile::getPath() const
{
    return m_pathToFile;
}

void NbtTreeItemNbtFile::fetchMore()
{
    m_canFetchData = false;

    // Read NBT data
    std::string filename = (m_pathToFile + "/" + m_filename).toStdString();
    m_nbtRootTag = amc::readNbtFile(filename);
    if(m_nbtRootTag) {
        amc::CompoundTag *tag = amc::tag_cast<amc::CompoundTag*>(m_nbtRootTag.get());
        addNbtChild(this, tag);
        /*for(amc::AbstractTag *childTag : tag->getValue()) {
            addNbtChild(this, childTag);
        }*/

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
    newItem->m_nbtRootTag = std::make_unique<amc::CompoundTag>();
    addNbtChild(newItem, newItem->m_nbtRootTag.get());

    return newItem;
}

} // namespace anv