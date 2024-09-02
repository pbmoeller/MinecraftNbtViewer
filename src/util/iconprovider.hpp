#ifndef AWESOMENBTVIEWER_UTIL_ICONPROVIDER_HPP
#define AWESOMENBTVIEWER_UTIL_ICONPROVIDER_HPP

// AwesomeMC
#include <cpp-anvil/nbt/types.hpp>

// Qt
#include <QIcon>

namespace anv
{

class IconProvider
{
public:
    enum IconSize
    {
        Size16 = 16,
        Size24 = 24,
        Size32 = 32,
    };

    enum IconType
    {
        ByteArrayTag,
        ByteTag,
        Chunk,
        CompoundTag,
        Copy,
        Cut,
        Delete,
        DirtBlock,
        DoubleTag,
        Edit,
        EmptyFile,
        Exit,
        FloatTag,
        Folder,
        IntArrayTag,
        IntTag,
        ListTag,
        LongArrayTag,
        LongTag,
        MoveDown,
        MoveUp,
        NbtFile,
        NewNbtFile,
        OpenFile,
        Paste,
        Refresh,
        Region,
        Rename,
        Save,
        ShortTag,
        StringTag,
        World,
    };

public:
    IconProvider();

    static QIcon icon(IconType iconType, IconSize iconSize = Size16);
    static QIcon icon(anvil::TagType tagType, IconSize iconSize = Size16);
};

} // namespace anv

#endif // AWESOMENBTVIEWER_UTIL_ICONPROVIDER_HPP
