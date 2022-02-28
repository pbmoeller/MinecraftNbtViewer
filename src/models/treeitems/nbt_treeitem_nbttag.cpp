// AwesomeNbtViewer
#include "nbt_treeitem_nbttag.hpp"

namespace anv
{

NbtTreeItemNbtTag::NbtTreeItemNbtTag(NbtTreeItemBase *parentItem, amc::AbstractTag *tag)
    : NbtTreeItemBase(parentItem)
    , m_tag(tag)
{

}

NbtTreeItemNbtTag::~NbtTreeItemNbtTag()
{

}

QIcon NbtTreeItemNbtTag::getIcon() const
{
    return QIcon(":/icons/16x16/EmptyFile.png");
}

QString NbtTreeItemNbtTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

amc::TagType NbtTreeItemNbtTag::getTagType() const
{
    return m_tag->getType();
}


NbtTreeItemByteTag::NbtTreeItemByteTag(NbtTreeItemBase *parentItem,
                                       amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}

NbtTreeItemByteTag::~NbtTreeItemByteTag()
{

}

QIcon NbtTreeItemByteTag::getIcon() const
{
    return QIcon(":/icons/16x16/ByteTag.png");
}

QString NbtTreeItemByteTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

NbtTreeItemShortTag::NbtTreeItemShortTag(NbtTreeItemBase *parentItem,
                                         amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}

NbtTreeItemShortTag::~NbtTreeItemShortTag()
{

}

QIcon NbtTreeItemShortTag::getIcon() const
{
    return QIcon(":/icons/16x16/ShortTag.png");
}

QString NbtTreeItemShortTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

NbtTreeItemIntTag::NbtTreeItemIntTag(NbtTreeItemBase *parentItem,
                                     amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}

NbtTreeItemIntTag::~NbtTreeItemIntTag()
{

}

QIcon NbtTreeItemIntTag::getIcon() const
{
    return QIcon(":/icons/16x16/IntTag.png");
}

QString NbtTreeItemIntTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

NbtTreeItemLongTag::NbtTreeItemLongTag(NbtTreeItemBase *parentItem,
                                       amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}

NbtTreeItemLongTag::~NbtTreeItemLongTag()
{

}

QIcon NbtTreeItemLongTag::getIcon() const
{
    return QIcon(":/icons/16x16/LongTag.png");
}

QString NbtTreeItemLongTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

NbtTreeItemFloatTag::NbtTreeItemFloatTag(NbtTreeItemBase *parentItem,
                                         amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}
    
NbtTreeItemFloatTag::~NbtTreeItemFloatTag()
{

}

QIcon NbtTreeItemFloatTag::getIcon() const
{
    return QIcon(":/icons/16x16/FloatTag.png");
}

QString NbtTreeItemFloatTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

NbtTreeItemDoubleTag::NbtTreeItemDoubleTag(NbtTreeItemBase *parentItem,
                                           amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}

NbtTreeItemDoubleTag::~NbtTreeItemDoubleTag()
{

}

QIcon NbtTreeItemDoubleTag::getIcon() const
{
    return QIcon(":/icons/16x16/DoubleTag.png");
}

QString NbtTreeItemDoubleTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

NbtTreeItemStringTag::NbtTreeItemStringTag(NbtTreeItemBase *parentItem,
                                           amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}

NbtTreeItemStringTag::~NbtTreeItemStringTag()
{

}

QIcon NbtTreeItemStringTag::getIcon() const
{
    return QIcon(":/icons/16x16/StringTag.png");
}

QString NbtTreeItemStringTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

NbtTreeItemCompoundTag::NbtTreeItemCompoundTag(NbtTreeItemBase *parentItem,
                                               amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}

NbtTreeItemCompoundTag::~NbtTreeItemCompoundTag()
{

}

QIcon NbtTreeItemCompoundTag::getIcon() const
{
    return QIcon(":/icons/16x16/CompoundTag.png");
}

QString NbtTreeItemCompoundTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

NbtTreeItemListTag::NbtTreeItemListTag(NbtTreeItemBase *parentItem,
                                       amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}

NbtTreeItemListTag::~NbtTreeItemListTag()
{

}

QIcon NbtTreeItemListTag::getIcon() const
{
    return QIcon(":/icons/16x16/ListTag.png");
}

QString NbtTreeItemListTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

NbtTreeItemByteArrayTag::NbtTreeItemByteArrayTag(NbtTreeItemBase *parentItem,
                                                 amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}

NbtTreeItemByteArrayTag::~NbtTreeItemByteArrayTag()
{

}

QIcon NbtTreeItemByteArrayTag::getIcon() const
{
    return QIcon(":/icons/16x16/ByteArrayTag.png");
}

QString NbtTreeItemByteArrayTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

NbtTreeItemIntArrayTag::NbtTreeItemIntArrayTag(NbtTreeItemBase *parentItem,
                                               amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}

NbtTreeItemIntArrayTag::~NbtTreeItemIntArrayTag()
{

}

QIcon NbtTreeItemIntArrayTag::getIcon() const
{
    return QIcon(":/icons/16x16/IntArrayTag.png");
}

QString NbtTreeItemIntArrayTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

NbtTreeItemLongArrayTag::NbtTreeItemLongArrayTag(NbtTreeItemBase *parentItem,
                                                 amc::AbstractTag *tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{

}

NbtTreeItemLongArrayTag::~NbtTreeItemLongArrayTag()
{

}

QIcon NbtTreeItemLongArrayTag::getIcon() const
{
    return QIcon(":/icons/16x16/LongArrayTag.png");
}

QString NbtTreeItemLongArrayTag::getName() const
{
    return QString(m_tag->getName().c_str());
}

} // namespace anv