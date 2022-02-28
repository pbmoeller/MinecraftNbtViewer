// AwesomeNbtViewer
#include "treeitem_util.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/tags/tags.hpp>

namespace anv
{

void addNbtChild(NbtTreeItemBase *parentItem,
                 amc::AbstractTag *tag)
{
    
    switch(tag->getType())
    {
        case amc::TagType::Byte:
        {
            new NbtTreeItemByteTag(parentItem, tag);
            break;
        }
        case amc::TagType::Short:
        {
            new NbtTreeItemShortTag(parentItem, tag);
            break;
        }
        case amc::TagType::Int:
        {
            new NbtTreeItemIntTag(parentItem, tag);
            break;
        }
        case amc::TagType::Long:
        {
            new NbtTreeItemLongTag(parentItem, tag);
            break;
        }
        case amc::TagType::Float:
        {
            new NbtTreeItemFloatTag(parentItem, tag);
            break;
        }
        case amc::TagType::Double:
        {
            new NbtTreeItemDoubleTag(parentItem, tag);
            break;
        }
        case amc::TagType::String:
        {
            new NbtTreeItemStringTag(parentItem, tag);
            break;
        }
        case amc::TagType::List:
        {
            NbtTreeItemListTag *treeItem = new NbtTreeItemListTag(parentItem, tag);
            for(amc::AbstractTag *childTag : amc::tag_cast<amc::ListTag*>(tag)->getValue()) {
                addNbtChild(treeItem, childTag);
            }
            treeItem->sort();
            break;
        }
        case amc::TagType::Compound:
        {
            NbtTreeItemCompoundTag *treeItem = new NbtTreeItemCompoundTag(parentItem, tag);
            for(amc::AbstractTag *childTag : amc::tag_cast<amc::CompoundTag*>(tag)->getValue()) {
                addNbtChild(treeItem, childTag);
            }
            treeItem->sort();
            break;
        }
        case amc::TagType::ByteArray:
        {
            new NbtTreeItemByteArrayTag(parentItem, tag);
            break;
        }
        case amc::TagType::IntArray:
        {
            new NbtTreeItemIntArrayTag(parentItem, tag);
            break;
        }
        case amc::TagType::LongArray:
        {
            new NbtTreeItemLongArrayTag(parentItem, tag);
            break;
        }
        case amc::TagType::End:
        {
            break;
        }
        default:
        case amc::TagType::Unknown:
        {
            break;
        }
    }

    return;
}

} // namespace anv