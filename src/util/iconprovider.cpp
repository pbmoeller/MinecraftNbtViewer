// MinecraftNbtViewer
#include "iconprovider.hpp"

namespace minecraft {
namespace nbt {

IconProvider::IconProvider()
{

}

QIcon IconProvider::icon(IconType iconType, IconSize iconSize)
{
    if(iconSize == Size16) {
        switch(iconType) {
            case ByteArrayTag:
                return QIcon(":/icons/16x16/ByteArrayTag.png");
            case ByteTag:
                return QIcon(":/icons/16x16/ByteTag.png");
            case Chunk:
                return QIcon(":/icons/16x16/Chunk.png");
            case CompoundTag:
                return QIcon(":/icons/16x16/CompoundTag.png");
            case Copy:
                return QIcon(":/icons/16x16/Copy.png");
            case Cut:
                return QIcon(":/icons/16x16/Cut.png");
            case Delete:
                return QIcon(":/icons/16x16/Delete.png");
            case DirtBlock:
                return QIcon(":/icons/16x16/DirtBlock.png");
            case DoubleTag:
                return QIcon(":/icons/16x16/DoubleTag.png");
            case Edit:
                return QIcon(":/icons/16x16/Edit.png");
            case EmptyFile:
                return QIcon(":/icons/16x16/EmptyFile.png");
            case Exit:
                return QIcon(":/icons/16x16/Exit.png");
            case FloatTag:
                return QIcon(":/icons/16x16/FloatTag.png");
            case Folder:
                return QIcon(":/icons/16x16/Folder.png");
            case IntArrayTag:
                return QIcon(":/icons/16x16/IntArrayTag.png");
            case IntTag:
                return QIcon(":/icons/16x16/IntTag.png");
            case ListTag:
                return QIcon(":/icons/16x16/ListTag.png");
            case LongArrayTag:
                return QIcon(":/icons/16x16/LongArrayTag.png");
            case LongTag:
                return QIcon(":/icons/16x16/LongTag.png");
            case MoveDown:
                return QIcon(":/icons/16x16/MoveDown.png");
            case MoveUp:
                return QIcon(":/icons/16x16/MoveUp.png");
            case NbtFile:
                return QIcon(":/icons/16x16/NbtFile.png");
            case NewNbtFile:
                return QIcon(":/icons/16x16/NewNbtFile.png");
            case OpenFile:
                return QIcon(":/icons/16x16/OpenFile.png");
            case Paste:
                return QIcon(":/icons/16x16/Paste.png");
            case Refresh:
                return QIcon(":/icons/16x16/Refresh.png");
            case Region:
                return QIcon(":/icons/16x16/Region.png");
            case Rename:
                return QIcon(":/icons/16x16/Rename.png");
            case Save:
                return QIcon(":/icons/16x16/Save.png");
            case ShortTag:
                return QIcon(":/icons/16x16/ShortTag.png");
            case StringTag:
                return QIcon(":/icons/16x16/StringTag.png");
            case World:
                return QIcon(":/icons/16x16/World.png");
        }
    } else if(iconSize == Size24) {
        switch(iconType) {
            case ByteArrayTag:
                return QIcon(":/icons/24x24/ByteArrayTag.png");
            case ByteTag:
                return QIcon(":/icons/24x24/ByteTag.png");
            case Chunk:
                return QIcon(":/icons/24x24/Chunk.png");
            case CompoundTag:
                return QIcon(":/icons/24x24/CompoundTag.png");
            case Copy:
                return QIcon(":/icons/24x24/Copy.png");
            case Cut:
                return QIcon(":/icons/24x24/Cut.png");
            case Delete:
                return QIcon(":/icons/24x24/Delete.png");
            case DirtBlock:
                return QIcon(":/icons/24x24/DirtBlock.png");
            case DoubleTag:
                return QIcon(":/icons/24x24/DoubleTag.png");
            case Edit:
                return QIcon(":/icons/24x24/Edit.png");
            case EmptyFile:
                return QIcon(":/icons/24x24/EmptyFile.png");
            case Exit:
                return QIcon(":/icons/24x24/Exit.png");
            case FloatTag:
                return QIcon(":/icons/24x24/FloatTag.png");
            case Folder:
                return QIcon(":/icons/24x24/Folder.png");
            case IntArrayTag:
                return QIcon(":/icons/24x24/IntArrayTag.png");
            case IntTag:
                return QIcon(":/icons/24x24/IntTag.png");
            case ListTag:
                return QIcon(":/icons/24x24/ListTag.png");
            case LongArrayTag:
                return QIcon(":/icons/24x24/LongArrayTag.png");
            case LongTag:
                return QIcon(":/icons/24x24/LongTag.png");
            case MoveDown:
                return QIcon(":/icons/24x24/MoveDown.png");
            case MoveUp:
                return QIcon(":/icons/24x24/MoveUp.png");
            case NbtFile:
                return QIcon(":/icons/24x24/NbtFile.png");
            case NewNbtFile:
                return QIcon(":/icons/24x24/NewNbtFile.png");
            case OpenFile:
                return QIcon(":/icons/24x24/OpenFile.png");
            case Paste:
                return QIcon(":/icons/24x24/Paste.png");
            case Refresh:
                return QIcon(":/icons/24x24/Refresh.png");
            case Region:
                return QIcon(":/icons/24x24/Region.png");
            case Rename:
                return QIcon(":/icons/24x24/Rename.png");
            case Save:
                return QIcon(":/icons/24x24/Save.png");
            case ShortTag:
                return QIcon(":/icons/24x24/ShortTag.png");
            case StringTag:
                return QIcon(":/icons/24x24/StringTag.png");
            case World:
                return QIcon(":/icons/24x24/World.png");
        }
    } else if(iconSize == Size32) {
        switch(iconType) {
            case ByteArrayTag:
                return QIcon(":/icons/32x32/ByteArrayTag.png");
            case ByteTag:
                return QIcon(":/icons/32x32/ByteTag.png");
            case Chunk:
                return QIcon(":/icons/32x32/Chunk.png");
            case CompoundTag:
                return QIcon(":/icons/32x32/CompoundTag.png");
            case Copy:
                return QIcon(":/icons/32x32/Copy.png");
            case Cut:
                return QIcon(":/icons/32x32/Cut.png");
            case Delete:
                return QIcon(":/icons/32x32/Delete.png");
            case DirtBlock:
                return QIcon(":/icons/32x32/DirtBlock.png");
            case DoubleTag:
                return QIcon(":/icons/32x32/DoubleTag.png");
            case Edit:
                return QIcon(":/icons/32x32/Edit.png");
            case EmptyFile:
                return QIcon(":/icons/32x32/EmptyFile.png");
            case Exit:
                return QIcon(":/icons/32x32/Exit.png");
            case FloatTag:
                return QIcon(":/icons/32x32/FloatTag.png");
            case Folder:
                return QIcon(":/icons/32x32/Folder.png");
            case IntArrayTag:
                return QIcon(":/icons/32x32/IntArrayTag.png");
            case IntTag:
                return QIcon(":/icons/32x32/IntTag.png");
            case ListTag:
                return QIcon(":/icons/32x32/ListTag.png");
            case LongArrayTag:
                return QIcon(":/icons/32x32/LongArrayTag.png");
            case LongTag:
                return QIcon(":/icons/32x32/LongTag.png");
            case MoveDown:
                return QIcon(":/icons/32x32/MoveDown.png");
            case MoveUp:
                return QIcon(":/icons/32x32/MoveUp.png");
            case NbtFile:
                return QIcon(":/icons/32x32/NbtFile.png");
            case NewNbtFile:
                return QIcon(":/icons/32x32/NewNbtFile.png");
            case OpenFile:
                return QIcon(":/icons/32x32/OpenFile.png");
            case Paste:
                return QIcon(":/icons/32x32/Paste.png");
            case Refresh:
                return QIcon(":/icons/32x32/Refresh.png");
            case Region:
                return QIcon(":/icons/32x32/Region.png");
            case Rename:
                return QIcon(":/icons/32x32/Rename.png");
            case Save:
                return QIcon(":/icons/32x32/Save.png");
            case ShortTag:
                return QIcon(":/icons/32x32/ShortTag.png");
            case StringTag:
                return QIcon(":/icons/32x32/StringTag.png");
            case World:
                return QIcon(":/icons/32x32/World.png");
        }
    }

    return QIcon();
}

QIcon IconProvider::icon(anvil::TagType tagType, IconSize iconSize)
{
    switch(tagType) {
        case anvil::TagType::Byte:
            return icon(ByteTag, iconSize);
        case anvil::TagType::Short:
            return icon(ShortTag, iconSize);
        case anvil::TagType::Int:
            return icon(IntTag, iconSize);
        case anvil::TagType::Long:
            return icon(LongTag, iconSize);
        case anvil::TagType::Float:
            return icon(FloatTag, iconSize);
        case anvil::TagType::Double:
            return icon(DoubleTag, iconSize);
        case anvil::TagType::ByteArray:
            return icon(ByteArrayTag, iconSize);
        case anvil::TagType::String:
            return icon(StringTag, iconSize);
        case anvil::TagType::List:
            return icon(ListTag, iconSize);
        case anvil::TagType::Compound:
            return icon(CompoundTag, iconSize);
        case anvil::TagType::IntArray:
            return icon(IntArrayTag, iconSize);
        case anvil::TagType::LongArray:
            return icon(LongArrayTag, iconSize);
        case anvil::TagType::End:
        case anvil::TagType::Unknown:
        default:
            return icon(EmptyFile, iconSize);
    }
}

} // namespace nbt
} // namespace minecraft
