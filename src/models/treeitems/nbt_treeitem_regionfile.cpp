//AwesomeNbtViewer
#include "nbt_treeitem_regionfile.hpp"
#include "nbt_treeitem_regionchunk.hpp"

namespace anv
{

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

QString NbtTreeItemRegionFile::getName() const
{
    return m_filename;
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
    m_region = std::make_unique<amc::Region>();
    m_region->loadPartiallyFromFile(filename);
    for(unsigned int index = 0; index < amc::ChunkCount; ++index)
    {
        amc::ChunkInfo &info = m_region->getRegionHeader().getChunkInfoAt(index);
        if(!info.isEmpty()) {
            new NbtTreeItemRegionChunk(this, m_region.get(), index);
        }
    }
}

} // namespace anv