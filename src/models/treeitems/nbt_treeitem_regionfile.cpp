// MinecraftNbtViewer
#include "nbt_treeitem_regionfile.hpp"
#include "nbt_treeitem_regionchunk.hpp"

namespace anv {

NbtTreeItemRegionFile::NbtTreeItemRegionFile(NbtTreeItemBase *parentItem,
                                             const QString &filename,
                                             const QString &pathToFile)
    : NbtTreeItemBase(parentItem)
    , m_canFetchData(true)
    , m_filename(filename)
    , m_pathToFile(pathToFile)
{

}

NbtTreeItemRegionFile::~NbtTreeItemRegionFile()
{

}

QIcon NbtTreeItemRegionFile::getIcon() const
{
    return QIcon(":/icons/16x16/Region.png");
}

QString NbtTreeItemRegionFile::getLabel() const
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

QString NbtTreeItemRegionFile::getPath() const
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
    m_region = std::make_unique<anvil::Region>();
    m_region->loadPartiallyFromFile(filename);
    for(unsigned int index = 0; index < anvil::Region::Chunks; ++index)
    {
        if(m_region->isChunkLoadable(index)) {
            new NbtTreeItemRegionChunk(this, m_region.get(), index);
        }
    }
}

} // namespace anv
