// AwesomeNbtViewer
#include "nbt_treeitem_regionchunk.hpp"
#include "treeitem_util.hpp"

// AwesomeMC
#include <cpp-anvil/anvil/coordinates.hpp>
//#include <AwesomeMC/util/conversion.hpp>

namespace anv
{

NbtTreeItemRegionChunk::NbtTreeItemRegionChunk(NbtTreeItemBase *parentItem,
                                               anvil::Region *region,
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
    anvil::Vec2 chunkCoord = m_parentRegion->fromIndex(m_index);
    anvil::Vec2 regionCoord = m_parentRegion->xz();
    anvil::Vec2 worldCoord = anvil::chunkRegion2ChunkWorld(chunkCoord, regionCoord);

    return QString("Chunk [%1, %2]    -> in world (%3, %4)")
        .arg(chunkCoord.x).arg(chunkCoord.z).arg(worldCoord.x).arg(worldCoord.z);
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
    anvil::Chunk &chunk = m_parentRegion->chunkAt(m_index);
    anvil::CompoundTag *tag = chunk.rootTag();
    if(tag) {
        for(std::unique_ptr<anvil::BasicTag> &childTag : tag->value()) {
            addNbtChild(this, childTag.get());
        }
        sort();
    }
}

}