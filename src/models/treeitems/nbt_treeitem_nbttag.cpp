// AwesomeNbtViewer
#include "nbt_treeitem_nbttag.hpp"
#include "nbt_treeitem_nbtfile.hpp"
#include "nbt_treeitem_regionfile.hpp"
#include "treeitem_util.hpp"
#include "models/nbt_data_treemodel.hpp"
#include "util/tag_mime_data.hpp"
#include "widgets/edit_dialog.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/tags/tags.hpp>

// Qt
#include <QObject>
#include <QApplication>
#include <QClipboard>

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

amc::AbstractTag* NbtTreeItemNbtTag::getTag()
{
    return m_tag;
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

void NbtTreeItemNbtTag::openRenameDialog(NbtDataTreeModel *model)
{
    EditDialog editDialog(this, model, EditDialog::EditFunction::Rename);
    editDialog.exec();
}

bool NbtTreeItemNbtTag::canEdit() const
{
    return true;
}

void NbtTreeItemNbtTag::openEditDialog(NbtDataTreeModel *model)
{
    EditDialog editDialog(this, model, EditDialog::EditFunction::EditValue);
    editDialog.exec();
}

bool NbtTreeItemNbtTag::canDelete() const
{
    return true;
}

void NbtTreeItemNbtTag::deleteTag()
{
    delete m_tag;
    m_tag = nullptr;
}

void NbtTreeItemNbtTag::deleteChildTag(amc::AbstractTag *tag)
{
    Q_UNUSED(tag);
}

bool NbtTreeItemNbtTag::canCut() const
{
    return true;
}

void NbtTreeItemNbtTag::cut()
{
    NbtTreeItemBase *parentItem = getParent();
    amc::AbstractTag *tag = nullptr;

    // Check if parent is ListTag or CompoundTag
    // If one of these Items take NbtTag
    NbtTreeItemListTag *parentListTag = dynamic_cast<NbtTreeItemListTag*>(parentItem);
    NbtTreeItemCompoundTag *parentCompoundTag = dynamic_cast<NbtTreeItemCompoundTag*>(parentItem);
    if(parentListTag) {
        amc::ListTag *listTag = dynamic_cast<amc::ListTag*>(parentListTag->getTag());
        tag = listTag->take(m_tag);
    } else if(parentCompoundTag) {
        amc::CompoundTag *compoundTag = dynamic_cast<amc::CompoundTag*>(parentCompoundTag->getTag());
        tag = compoundTag->take(m_tag);
    } else {
        return;
    }

    // Item to clipboard
    std::shared_ptr<amc::AbstractTag> data(tag);

    TagMimeData *mimeData = new TagMimeData();
    mimeData->setData(TagMimeData::TagMimeType, data);
    qApp->clipboard()->setMimeData(mimeData);

    // delete this object
    delete this;
}

bool NbtTreeItemNbtTag::canCopy() const
{
    return true;
}

void NbtTreeItemNbtTag::copy()
{
    std::shared_ptr<amc::AbstractTag> data(m_tag->clone());

    TagMimeData *mimeData = new TagMimeData();
    mimeData->setData(TagMimeData::TagMimeType, data);
    qApp->clipboard()->setMimeData(mimeData);
}

bool NbtTreeItemNbtTag::canMoveUp() const
{
    // Check if we have a parent
    NbtTreeItemNbtTag *parentItem = dynamic_cast<NbtTreeItemNbtTag*>(m_parent);
    if(parentItem) {
        // Check if tag is valid and is of type List
        amc::AbstractTag *parentTag = parentItem->getTag();
        if(parentTag && parentTag->getType() == amc::TagType::List) {
            amc::ListTag *listTag = tag_cast<amc::ListTag*>(parentTag);

            // Now check if the moving makes sense
            if(listTag->indexOf(m_tag) > 0) {
                return true;
            }
        }
    }
    return false;
}

bool NbtTreeItemNbtTag::canMoveDown() const
{
    // Check if we have a parent
    NbtTreeItemNbtTag *parentItem = dynamic_cast<NbtTreeItemNbtTag*>(m_parent);
    if(parentItem) {
        // Check if tag is valid and is of type List
        amc::AbstractTag *parentTag = parentItem->getTag();
        if(parentTag && parentTag->getType() == amc::TagType::List) {
            amc::ListTag *listTag = tag_cast<amc::ListTag*>(parentTag);

            // Now check if the moving makes sense
            int64_t idx = listTag->indexOf(m_tag);
            if(idx >= 0 && idx < (listTag->size() - 1)) {
                return true;
            }
        }
    }
    return false;
}

void NbtTreeItemNbtTag::sort()
{
    std::sort(m_children.begin(),
              m_children.end(),
              &treeItemNbtTagCompare);
}

NbtTreeItemBase* NbtTreeItemNbtTag::markItemDirty()
{
    NbtTreeItemBase *item = this;

    // Walk up the tree until we find a TreeItem which is not a NBT tag.
    while(item != nullptr) {
        if(dynamic_cast<NbtTreeItemNbtFile*>(item)) {
            return item;
        }
        if(dynamic_cast<NbtTreeItemRegionFile*>(item)) {
            return item;
        }
        item = item->getParent();
    }
    return nullptr;
}

/// ByteTag
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

/// ShortTag
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

/// IntTag
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

/// LongTag
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

/// FloatTag
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

/// DoubleTag
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

/// StringTag
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

/// ListTag & CompoundTag Helper
void pasteHelper(NbtTreeItemNbtTag *item)
{
    const QMimeData *mimeData = qApp->clipboard()->mimeData();
    const TagMimeData *tagMimeData = dynamic_cast<const TagMimeData*>(mimeData);
    if(tagMimeData && tagMimeData->hasFormat(TagMimeData::TagMimeType)) {
        std::shared_ptr<amc::AbstractTag> tagData = tagMimeData->toTagData();
        amc::AbstractVectorTag *vectorTag = dynamic_cast<amc::AbstractVectorTag*>(item->getTag());
        if(tagData && vectorTag) {
            // Create a new ownership and transfer it to CompoundTag.
            // This is necessary when pasting again and if the shared_ptr get deleted.
            amc::AbstractTag *newTag = tagData->clone();
            vectorTag->pushBack(newTag);

            // Now cerate new TreeItems from new data
            addNbtChild(item, newTag);
        }
    }
}

/// CompoundTag
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

void NbtTreeItemCompoundTag::deleteChildTag(amc::AbstractTag *tag)
{
    tag_cast<amc::CompoundTag*>(m_tag)->erase(tag);
}

bool NbtTreeItemCompoundTag::canPaste() const
{
    const QMimeData *mimeData = qApp->clipboard()->mimeData();
    return mimeData->hasFormat(TagMimeData::TagMimeType);
}

void NbtTreeItemCompoundTag::paste()
{
    pasteHelper(this);
}

/// ListTag
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

void NbtTreeItemListTag::swap(int indexA, int indexB)
{
    amc::ListTag *listTag = dynamic_cast<amc::ListTag*>(m_tag);
    if(listTag) {
        assert(indexA >= 0 && indexA < listTag->size());
        assert(indexB >= 0 && indexB < listTag->size());
        listTag->swap(indexA, indexB);
        m_children.swapItemsAt(indexA, indexB);
    }
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

void NbtTreeItemListTag::deleteChildTag(amc::AbstractTag *tag)
{
    tag_cast<amc::ListTag*>(m_tag)->erase(tag);
}

bool NbtTreeItemListTag::canPaste() const
{
    const QMimeData *mimeData = qApp->clipboard()->mimeData();
    const TagMimeData *tagMimeData = dynamic_cast<const TagMimeData*>(mimeData);
    if(mimeData && mimeData->hasFormat(TagMimeData::TagMimeType)) {
        amc::ListTag *listTag = dynamic_cast<amc::ListTag*>(m_tag);
        if(listTag) {
            if(listTag->size() == 0) {
                return true;
            }
            if(tagMimeData) {
                std::shared_ptr<amc::AbstractTag> tagData = tagMimeData->toTagData();
                if(tagData && listTag->getListType() == tagData->getType()) {
                    return true;
                }
            }
        }
    }

    return false;
}

void NbtTreeItemListTag::paste()
{
    pasteHelper(this);
}

/// ByteArrayTag
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

/// IntArrayTag
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

/// LongArrayTag
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