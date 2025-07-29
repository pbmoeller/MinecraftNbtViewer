// MinecraftNbtViewer
#include "nbt_treeitem_regionfile.hpp"
#include "nbt_treeitem_regionchunk.hpp"

namespace minecraft {
namespace nbt {

NbtTreeItemRegionFile::NbtTreeItemRegionFile(NbtTreeItemBase* parentItem, const QString& filename,
                                             const QString& pathToFile)
    : NbtTreeItemBase(parentItem)
    , m_canFetchData(true)
    , m_filename(filename)
    , m_pathToFile(pathToFile)
{ }

NbtTreeItemRegionFile::~NbtTreeItemRegionFile() = default;

QIcon NbtTreeItemRegionFile::icon() const
{
    return QIcon(":/icons/16x16/Region.png");
}

QString NbtTreeItemRegionFile::name() const
{
    return m_filename;
}

QString NbtTreeItemRegionFile::label() const
{
    return m_filename;
}

bool NbtTreeItemRegionFile::canRefresh() const
{
    return true;
}

bool NbtTreeItemRegionFile::canOpenInExplorer() const
{
    return true;
}

QString NbtTreeItemRegionFile::path() const
{
    return m_pathToFile;
}

bool NbtTreeItemRegionFile::canFetchMore() const
{
    return m_canFetchData;
}

void NbtTreeItemRegionFile::fetchMore()
{
    m_canFetchData = false;

    // Initialize Region
    std::string filename = (m_pathToFile + "/" + m_filename).toStdString();
    m_region             = std::make_unique<anvil::Region>();
    m_region->loadPartiallyFromFile(filename);
    for(unsigned int index = 0; index < anvil::Region::Chunks; ++index) {
        if(m_region->isChunkLoadable(index)) {
            new NbtTreeItemRegionChunk(this, m_region.get(), index);
        }
    }
}

} // namespace nbt
} // namespace minecraft
