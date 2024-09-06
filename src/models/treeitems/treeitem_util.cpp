// MinecraftNbtViewer
#include "treeitem_util.hpp"

// cpp-anvil
#include <cpp-anvil/nbt.hpp>

namespace minecraft {
namespace nbt {

void addNbtChild(NbtTreeItemBase *parentItem,
                 anvil::BasicTag *tag)
{
    switch(tag->type())
    {
        case anvil::TagType::Byte:
        {
            new NbtTreeItemByteTag(parentItem, tag);
            break;
        }
        case anvil::TagType::Short:
        {
            new NbtTreeItemShortTag(parentItem, tag);
            break;
        }
        case anvil::TagType::Int:
        {
            new NbtTreeItemIntTag(parentItem, tag);
            break;
        }
        case anvil::TagType::Long:
        {
            new NbtTreeItemLongTag(parentItem, tag);
            break;
        }
        case anvil::TagType::Float:
        {
            new NbtTreeItemFloatTag(parentItem, tag);
            break;
        }
        case anvil::TagType::Double:
        {
            new NbtTreeItemDoubleTag(parentItem, tag);
            break;
        }
        case anvil::TagType::String:
        {
            new NbtTreeItemStringTag(parentItem, tag);
            break;
        }
        case anvil::TagType::List:
        {
            NbtTreeItemListTag *treeItem = new NbtTreeItemListTag(parentItem, tag);
            for(std::unique_ptr<anvil::BasicTag> &childTag : anvil::tag_cast<anvil::ListTag*>(tag)->value()) {
                addNbtChild(treeItem, childTag.get());
            }
            treeItem->sort();
            break;
        }
        case anvil::TagType::Compound:
        {
            NbtTreeItemCompoundTag *treeItem = new NbtTreeItemCompoundTag(parentItem, tag);
            for(std::unique_ptr<anvil::BasicTag> &childTag : anvil::tag_cast<anvil::CompoundTag*>(tag)->value()) {
                addNbtChild(treeItem, childTag.get());
            }
            treeItem->sort();
            break;
        }
        case anvil::TagType::ByteArray:
        {
            new NbtTreeItemByteArrayTag(parentItem, tag);
            break;
        }
        case anvil::TagType::IntArray:
        {
            new NbtTreeItemIntArrayTag(parentItem, tag);
            break;
        }
        case anvil::TagType::LongArray:
        {
            new NbtTreeItemLongArrayTag(parentItem, tag);
            break;
        }
        case anvil::TagType::End:
        {
            break;
        }
        case anvil::TagType::Unknown:
        default:
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
    if(leftTag->tagType() == anvil::TagType::Compound
       && rightTag->tagType() == anvil::TagType::Compound) {
        return leftTag->name().toLower() < rightTag->name().toLower();
    } else if(leftTag->tagType() == anvil::TagType::Compound) {
        return true;
    } else if(rightTag->tagType() == anvil::TagType::Compound) {
        return false;
    } else {
        if(leftTag->tagType() == anvil::TagType::List
           && rightTag->tagType() == anvil::TagType::List) {
            return leftTag->name().toLower() < rightTag->name().toLower();
        } else if(leftTag->tagType() == anvil::TagType::List) {
            return true;
        } else if(rightTag->tagType() == anvil::TagType::List) {
            return false;
        }
    }

    return leftTag->name().toLower() < rightTag->name().toLower();
}

} // namespace nbt
} // namespace minecraft
