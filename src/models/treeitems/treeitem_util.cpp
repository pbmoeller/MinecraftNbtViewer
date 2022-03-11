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

bool treeItemNbtTagCompare(NbtTreeItemBase *left,
                           NbtTreeItemBase *right)
{
    if(!left || !right) {
        return false;
    }

    NbtTreeItemNbtTag *leftTag = dynamic_cast<NbtTreeItemNbtTag*>(left);
    NbtTreeItemNbtTag *rightTag = dynamic_cast<NbtTreeItemNbtTag*>(right);

    if(leftTag != nullptr
       && rightTag == nullptr) {
        return true;
    }
    if(rightTag != nullptr
       && leftTag == nullptr) {
        return false;
    }
    if(leftTag->getTagType() == amc::TagType::Compound
       && rightTag->getTagType() == amc::TagType::Compound) {
        return leftTag->getLabel().toLower() < rightTag->getLabel().toLower();
    } else if(leftTag->getTagType() == amc::TagType::Compound) {
        return true;
    } else if(rightTag->getTagType() == amc::TagType::Compound) {
        return false;
    } else {
        if(leftTag->getTagType() == amc::TagType::List
           && rightTag->getTagType() == amc::TagType::List) {
            return leftTag->getLabel().toLower() < rightTag->getLabel().toLower();
        } else if(leftTag->getTagType() == amc::TagType::List) {
            return true;
        } else if(rightTag->getTagType() == amc::TagType::List) {
            return false;
        }
    }

    return leftTag->getLabel().toLower() < rightTag->getLabel().toLower();
}

} // namespace anv