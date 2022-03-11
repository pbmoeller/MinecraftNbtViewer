// AwesomeNbtViewer
#include "nbt_treeitem_regionchunk.hpp"
#include "treeitem_util.hpp"

// AwesomeMC
#include <AwesomeMC/util/conversion.hpp>

namespace anv
{

NbtTreeItemRegionChunk::NbtTreeItemRegionChunk(NbtTreeItemBase *parentItem,
                                               amc::Region *region,
                                               unsigned int index)
    : NbtTreeItemBase(parentItem)
    , m_canFetchData(true)
    , m_index(index)
    , m_parentRegion(region)
{

}

NbtTreeItemRegionChunk::~NbtTreeItemRegionChunk()
{

}

void NbtTreeItemRegionChunk::sort()
{
    std::sort(m_children.begin(),
              m_children.end(),
              &treeItemNbtTagCompare);
}

QIcon NbtTreeItemRegionChunk::getIcon() const
{
    return QIcon(":/icons/16x16/Chunk.png");
}

QString NbtTreeItemRegionChunk::getLabel() const
{
    auto [chunkX, chunkZ] = amc::xzFromChunkIndex(m_index);
    int wX;
    int wZ;
    amc::convertChunkRegion2ChunkWorld(chunkX, chunkZ, m_parentRegion->getX(), m_parentRegion->getZ(), wX, wZ);

    return QString("Chunk [%1, %2]    -> in world (%3, %4)").arg(chunkX).arg(chunkZ).arg(wX).arg(wZ);
}

bool NbtTreeItemRegionChunk::canFetchMore() const
{
    return m_canFetchData;
}

void NbtTreeItemRegionChunk::fetchMore()
{
    m_canFetchData = false;

    // Load Chunk in Region
    m_parentRegion->loadChunkAt(m_index);
    amc::Chunk &chunk = m_parentRegion->getChunkAt(m_index);
    amc::CompoundTag *tag = chunk.getRootTag();
    if(tag) {
        for(amc::AbstractTag *childTag : tag->getValue()) {
            addNbtChild(this, childTag);
        }
        sort();
    }
}

}