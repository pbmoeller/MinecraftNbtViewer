// AwesomeNbtViewer
#include "nbt_treeitem_nbttag.hpp"
#include "treeitem_util.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/tags/tags.hpp>

// Qt
#include <QObject>

namespace anv
{

NbtTreeItemNbtTag::NbtTreeItemNbtTag(NbtTreeItemBase *parentItem,
                                     amc::AbstractTag *tag)
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

QString NbtTreeItemNbtTag::getLabel() const
{
    return QString(m_tag->getName().c_str());
}

amc::TagType NbtTreeItemNbtTag::getTagType() const
{
    return m_tag->getType();
}

bool NbtTreeItemNbtTag::canRename() const
{
    // Only Tags that are not children of ListTag can be renamed.
    // (ListItems do not have names, however its theoretically possible to give them names.)
    return (dynamic_cast<NbtTreeItemListTag*>(m_parent) == nullptr);
}

void NbtTreeItemNbtTag::rename(const QString &name)
{
    m_tag->setName(name.toStdString());
}

void NbtTreeItemNbtTag::sort()
{
    std::sort(m_children.begin(),
              m_children.end(),
              &treeItemNbtTagCompare);
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

QString NbtTreeItemByteTag::getLabel() const
{
    amc::ByteTag *tag = amc::tag_cast<amc::ByteTag*>(m_tag);
    QString name;
    if(!tag->getName().empty()) {
        name = QString("%1: ").arg(tag->getName().c_str());
    }
    name += QString::number(tag->getValue());

    return name;
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

QString NbtTreeItemShortTag::getLabel() const
{
    amc::ShortTag *tag = amc::tag_cast<amc::ShortTag*>(m_tag);
    QString name;
    if(!tag->getName().empty()) {
        name = QString("%1: ").arg(tag->getName().c_str());
    }
    name += QString::number(tag->getValue());

    return name;
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

QString NbtTreeItemIntTag::getLabel() const
{
    amc::IntTag *tag = amc::tag_cast<amc::IntTag*>(m_tag);
    QString name;
    if(!tag->getName().empty()) {
        name = QString("%1: ").arg(tag->getName().c_str());
    }
    name += QString::number(tag->getValue());

    return name;
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

QString NbtTreeItemLongTag::getLabel() const
{
    amc::LongTag *tag = amc::tag_cast<amc::LongTag*>(m_tag);
    QString name;
    if(!tag->getName().empty()) {
        name = QString("%1: ").arg(tag->getName().c_str());
    }
    name += QString::number(tag->getValue());

    return name;
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

QString NbtTreeItemFloatTag::getLabel() const
{
    amc::FloatTag *tag = amc::tag_cast<amc::FloatTag*>(m_tag);
    QString name;
    if(!tag->getName().empty()) {
        name = QString("%1: ").arg(tag->getName().c_str());
    }
    name += QString::number(tag->getValue(), 'g', 6);

    return name;
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

QString NbtTreeItemDoubleTag::getLabel() const
{
    amc::DoubleTag *tag = amc::tag_cast<amc::DoubleTag*>(m_tag);
    QString name;
    if(!tag->getName().empty()) {
        name = QString("%1: ").arg(tag->getName().c_str());
    }
    name += QString::number(tag->getValue(), 'g', 16);

    return name;
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

QString NbtTreeItemStringTag::getLabel() const
{
    amc::StringTag *tag = amc::tag_cast<amc::StringTag*>(m_tag);
    QString name;
    if(!tag->getName().empty()) {
        name = QString(tag->getName().c_str()).append(": ");
    }
    name += tag->getValue().c_str();

    return name;
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

QString NbtTreeItemCompoundTag::getLabel() const
{
    return QString(QObject::tr("%1: %2 entries"))
                .arg(m_tag->getName().c_str())
                .arg(amc::tag_cast<amc::CompoundTag*>(m_tag)->size());
}

bool NbtTreeItemCompoundTag::canAddNbtTag(amc::TagType type) const
{
    Q_UNUSED(type);
    return true;
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

QString NbtTreeItemListTag::getLabel() const
{
    return QString(QObject::tr("%1: %2 entries"))
        .arg(m_tag->getName().c_str())
        .arg(amc::tag_cast<amc::ListTag*>(m_tag)->size());
}

bool NbtTreeItemListTag::canAddNbtTag(amc::TagType type) const
{
    if(m_children.size() > 0) {
        amc::ListTag *listTag = amc::tag_cast<amc::ListTag*>(m_tag);
        if(listTag && listTag->getListType() == type) {
            return true;
        }
        return false;
    }
    return true;
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

QString NbtTreeItemByteArrayTag::getLabel() const
{
    amc::ByteArrayTag *tag = amc::tag_cast<amc::ByteArrayTag*>(m_tag);
    QString name;
    if(!tag->getName().empty()) {
        name = QString("%1: ").arg(tag->getName().c_str());
    }
    name += QString("%1 Bytes").arg(tag->size());

    return name;
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

QString NbtTreeItemIntArrayTag::getLabel() const
{
    amc::IntArrayTag *tag = amc::tag_cast<amc::IntArrayTag*>(m_tag);
    QString name;
    if(!tag->getName().empty()) {
        name = QString("%1: ").arg(tag->getName().c_str());
    }
    name += QString("%1 Integers").arg(tag->size());

    return name;
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

QString NbtTreeItemLongArrayTag::getLabel() const
{
    amc::LongArrayTag *tag = amc::tag_cast<amc::LongArrayTag*>(m_tag);
    QString name;
    if(!tag->getName().empty()) {
        name = QString("%1: ").arg(tag->getName().c_str());
    }
    name += QString("%1 Long Integers").arg(tag->size());

    return name;
}

} // namespace anv