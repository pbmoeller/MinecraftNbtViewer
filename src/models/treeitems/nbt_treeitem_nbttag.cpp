// MinecraftNbtViewer
#include "nbt_treeitem_nbttag.hpp"
#include "models/nbt_data_treemodel.hpp"
#include "nbt_treeitem_nbtfile.hpp"
#include "nbt_treeitem_regionfile.hpp"
#include "treeitem_util.hpp"
#include "util/tag_mime_data.hpp"
#include "widgets/edit_dialog.hpp"

// Qt
#include <QApplication>
#include <QClipboard>
#include <QObject>

namespace minecraft {
namespace nbt {

NbtTreeItemNbtTag::NbtTreeItemNbtTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemBase(parentItem)
    , m_tag(tag)
{ }

NbtTreeItemNbtTag::~NbtTreeItemNbtTag() = default;

QIcon NbtTreeItemNbtTag::icon() const
{
    return QIcon(":/icons/16x16/EmptyFile.png");
}

QString NbtTreeItemNbtTag::name() const
{
    return QString(m_tag->name().c_str());
}

QString NbtTreeItemNbtTag::label() const
{
    return QString(m_tag->name().c_str());
}

anvil::TagType NbtTreeItemNbtTag::tagType() const
{
    return m_tag->type();
}

anvil::BasicTag* NbtTreeItemNbtTag::tag()
{
    return m_tag;
}

bool NbtTreeItemNbtTag::isNbtTag() const
{
    return true;
}

bool NbtTreeItemNbtTag::canRename() const
{
    // Only Tags that are not children of ListTag can be renamed.
    // (ListItems do not have names, however its theoretically possible to give them names.)
    return (dynamic_cast<NbtTreeItemListTag*>(m_parent) == nullptr);
}

void NbtTreeItemNbtTag::rename(const QString& name)
{
    m_tag->setName(name.toStdString());
}

void NbtTreeItemNbtTag::openRenameDialog(NbtDataTreeModel* model)
{
    EditDialog editDialog(this, model, EditDialog::EditFunction::Rename);
    editDialog.exec();
}

bool NbtTreeItemNbtTag::canEdit() const
{
    return true;
}

void NbtTreeItemNbtTag::openEditDialog(NbtDataTreeModel* model)
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

void NbtTreeItemNbtTag::deleteChildTag(anvil::BasicTag* tag)
{
    Q_UNUSED(tag);
}

bool NbtTreeItemNbtTag::canCut() const
{
    return true;
}

void NbtTreeItemNbtTag::cut()
{
    NbtTreeItemBase* parentItem = parent();
    std::unique_ptr<anvil::BasicTag> tag;

    // Check if parent is ListTag or CompoundTag
    // If one of these Items take NbtTag
    NbtTreeItemListTag* parentListTag         = dynamic_cast<NbtTreeItemListTag*>(parentItem);
    NbtTreeItemCompoundTag* parentCompoundTag = dynamic_cast<NbtTreeItemCompoundTag*>(parentItem);
    if(parentListTag) {
        anvil::ListTag* listTag = dynamic_cast<anvil::ListTag*>(parentListTag->tag());
        tag                     = listTag->take(m_tag);
    } else if(parentCompoundTag) {
        anvil::CompoundTag* compoundTag =
            dynamic_cast<anvil::CompoundTag*>(parentCompoundTag->tag());
        tag = compoundTag->take(m_tag);
    } else {
        return;
    }

    // Item to clipboard
    std::shared_ptr<anvil::BasicTag> data(std::move(tag));

    TagMimeData* mimeData = new TagMimeData();
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
    std::shared_ptr<anvil::BasicTag> data(m_tag->clone());

    TagMimeData* mimeData = new TagMimeData();
    mimeData->setData(TagMimeData::TagMimeType, data);
    qApp->clipboard()->setMimeData(mimeData);
}

bool NbtTreeItemNbtTag::canMoveUp() const
{
    // Check if we have a parent
    NbtTreeItemNbtTag* parentItem = dynamic_cast<NbtTreeItemNbtTag*>(m_parent);
    if(parentItem) {
        // Check if tag is valid and is of type List
        anvil::BasicTag* parentTag = parentItem->tag();
        if(parentTag && parentTag->type() == anvil::TagType::List) {
            anvil::ListTag* listTag = tag_cast<anvil::ListTag*>(parentTag);

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
    NbtTreeItemNbtTag* parentItem = dynamic_cast<NbtTreeItemNbtTag*>(m_parent);
    if(parentItem) {
        // Check if tag is valid and is of type List
        anvil::BasicTag* parentTag = parentItem->tag();
        if(parentTag && parentTag->type() == anvil::TagType::List) {
            anvil::ListTag* listTag = tag_cast<anvil::ListTag*>(parentTag);

            // Now check if the moving makes sense
            size_t idx = listTag->indexOf(m_tag);
            if(idx < (listTag->size() - 1)) {
                return true;
            }
        }
    }
    return false;
}

void NbtTreeItemNbtTag::sort()
{
    std::sort(m_children.begin(), m_children.end(), &treeItemNbtTagCompare);
}

NbtTreeItemBase* NbtTreeItemNbtTag::markItemDirty()
{
    NbtTreeItemBase* item = this;

    // Walk up the tree until we find a TreeItem which is not a NBT tag.
    while(item != nullptr) {
        if(dynamic_cast<NbtTreeItemNbtFile*>(item)) {
            return item;
        }
        if(dynamic_cast<NbtTreeItemRegionFile*>(item)) {
            return item;
        }
        item = item->parent();
    }
    return nullptr;
}

/// ByteTag
NbtTreeItemByteTag::NbtTreeItemByteTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemByteTag::~NbtTreeItemByteTag() = default;

QIcon NbtTreeItemByteTag::icon() const
{
    return QIcon(":/icons/16x16/ByteTag.png");
}

QString NbtTreeItemByteTag::label() const
{
    anvil::ByteTag* tag = anvil::tag_cast<anvil::ByteTag*>(m_tag);
    QString name;
    if(!tag->name().empty()) {
        name = QString("%1: ").arg(tag->name().c_str());
    }
    name += QString::number(tag->value());

    return name;
}

QString NbtTreeItemByteTag::value() const
{
    return QString::number(m_tag->asByteTag()->value());
}

/// ShortTag
NbtTreeItemShortTag::NbtTreeItemShortTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemShortTag::~NbtTreeItemShortTag() = default;

QIcon NbtTreeItemShortTag::icon() const
{
    return QIcon(":/icons/16x16/ShortTag.png");
}

QString NbtTreeItemShortTag::label() const
{
    anvil::ShortTag* tag = anvil::tag_cast<anvil::ShortTag*>(m_tag);
    QString name;
    if(!tag->name().empty()) {
        name = QString("%1: ").arg(tag->name().c_str());
    }
    name += QString::number(tag->value());

    return name;
}

QString NbtTreeItemShortTag::value() const
{
    return QString::number(m_tag->asShortTag()->value());
}

/// IntTag
NbtTreeItemIntTag::NbtTreeItemIntTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemIntTag::~NbtTreeItemIntTag() = default;

QIcon NbtTreeItemIntTag::icon() const
{
    return QIcon(":/icons/16x16/IntTag.png");
}

QString NbtTreeItemIntTag::label() const
{
    anvil::IntTag* tag = anvil::tag_cast<anvil::IntTag*>(m_tag);
    QString name;
    if(!tag->name().empty()) {
        name = QString("%1: ").arg(tag->name().c_str());
    }
    name += QString::number(tag->value());

    return name;
}

QString NbtTreeItemIntTag::value() const
{
    return QString::number(m_tag->asIntTag()->value());
}

/// LongTag
NbtTreeItemLongTag::NbtTreeItemLongTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemLongTag::~NbtTreeItemLongTag() = default;

QIcon NbtTreeItemLongTag::icon() const
{
    return QIcon(":/icons/16x16/LongTag.png");
}

QString NbtTreeItemLongTag::label() const
{
    anvil::LongTag* tag = anvil::tag_cast<anvil::LongTag*>(m_tag);
    QString name;
    if(!tag->name().empty()) {
        name = QString("%1: ").arg(tag->name().c_str());
    }
    name += QString::number(tag->value());

    return name;
}

QString NbtTreeItemLongTag::value() const
{
    return QString::number(m_tag->asLongTag()->value());
}

/// FloatTag
NbtTreeItemFloatTag::NbtTreeItemFloatTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemFloatTag::~NbtTreeItemFloatTag() = default;

QIcon NbtTreeItemFloatTag::icon() const
{
    return QIcon(":/icons/16x16/FloatTag.png");
}

QString NbtTreeItemFloatTag::label() const
{
    anvil::FloatTag* tag = anvil::tag_cast<anvil::FloatTag*>(m_tag);
    QString name;
    if(!tag->name().empty()) {
        name = QString("%1: ").arg(tag->name().c_str());
    }
    name += QLocale().toString(tag->value(), 'g', 6);

    return name;
}

QString NbtTreeItemFloatTag::value() const
{
    return QString::number(m_tag->asFloatTag()->value());
}

/// DoubleTag
NbtTreeItemDoubleTag::NbtTreeItemDoubleTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemDoubleTag::~NbtTreeItemDoubleTag() = default;

QIcon NbtTreeItemDoubleTag::icon() const
{
    return QIcon(":/icons/16x16/DoubleTag.png");
}

QString NbtTreeItemDoubleTag::label() const
{
    anvil::DoubleTag* tag = anvil::tag_cast<anvil::DoubleTag*>(m_tag);
    QString name;
    if(!tag->name().empty()) {
        name = QString("%1: ").arg(tag->name().c_str());
    }
    name += QLocale().toString(tag->value(), 'g', 16);

    return name;
}

QString NbtTreeItemDoubleTag::value() const
{
    return QString::number(m_tag->asDoubleTag()->value());
}

/// StringTag
NbtTreeItemStringTag::NbtTreeItemStringTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemStringTag::~NbtTreeItemStringTag() = default;

QIcon NbtTreeItemStringTag::icon() const
{
    return QIcon(":/icons/16x16/StringTag.png");
}

QString NbtTreeItemStringTag::label() const
{
    anvil::StringTag* tag = anvil::tag_cast<anvil::StringTag*>(m_tag);
    QString name;
    if(!tag->name().empty()) {
        name = QString(tag->name().c_str()).append(": ");
    }
    name += tag->value().c_str();

    return name;
}

QString NbtTreeItemStringTag::value() const
{
    return QString(m_tag->asStringTag()->value().c_str());
}

/// ListTag & CompoundTag Helper
void pasteHelper(NbtTreeItemNbtTag* item)
{
    const QMimeData* mimeData      = qApp->clipboard()->mimeData();
    const TagMimeData* tagMimeData = dynamic_cast<const TagMimeData*>(mimeData);
    if(tagMimeData && tagMimeData->hasFormat(TagMimeData::TagMimeType)) {
        std::shared_ptr<anvil::BasicTag> tagData = tagMimeData->toTagData();
        if(tagData && anvil::isCollectionTag(item->tagType())) {
            // Create a new ownership and transfer it to CompoundTag.
            // This is necessary when pasting again and if the shared_ptr get deleted.
            auto newTag = tagData->clone();

            // Check if the item is CompoundTag and if it already has a tag with the name.
            // The CompoundTag does not allow 2 items to have the same name -> rename.
            if(item->tagType() == anvil::TagType::Compound) {
                std::string name           = newTag->name();
                anvil::CompoundTag* parent = tag_cast<anvil::CompoundTag*>(item->tag());
                int index                  = 1;
                while(parent->hasChild(name)) {
                    name = newTag->name() + " (" + std::to_string(index) + ")";
                    ++index;
                }
                if(name != newTag->name()) {
                    newTag->setName(name);
                }
            }

            // Now create new TreeItems from new data
            addNbtChild(item, newTag.get());

            if(item->tagType() == anvil::TagType::List) {
                tag_cast<anvil::ListTag*>(item->tag())->push_back(std::move(newTag));
            } else if(item->tagType() == anvil::TagType::Compound) {
                tag_cast<anvil::CompoundTag*>(item->tag())->push_back(std::move(newTag));
            }
        }
    }
}

/// CompoundTag
NbtTreeItemCompoundTag::NbtTreeItemCompoundTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemCompoundTag::~NbtTreeItemCompoundTag() = default;

QIcon NbtTreeItemCompoundTag::icon() const
{
    return QIcon(":/icons/16x16/CompoundTag.png");
}

QString NbtTreeItemCompoundTag::label() const
{
    return QString(QObject::tr("%1: %2 entries"))
        .arg(m_tag->name().c_str())
        .arg(anvil::tag_cast<anvil::CompoundTag*>(m_tag)->size());
}

bool NbtTreeItemCompoundTag::canAddNbtTag(anvil::TagType type) const
{
    Q_UNUSED(type);
    return true;
}

void NbtTreeItemCompoundTag::deleteChildTag(anvil::BasicTag* tag)
{
    tag_cast<anvil::CompoundTag*>(m_tag)->erase(tag);
}

bool NbtTreeItemCompoundTag::canPaste() const
{
    const QMimeData* mimeData = qApp->clipboard()->mimeData();
    return mimeData->hasFormat(TagMimeData::TagMimeType);
}

void NbtTreeItemCompoundTag::paste()
{
    pasteHelper(this);
}

/// ListTag
NbtTreeItemListTag::NbtTreeItemListTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemListTag::~NbtTreeItemListTag() = default;

QIcon NbtTreeItemListTag::icon() const
{
    return QIcon(":/icons/16x16/ListTag.png");
}

QString NbtTreeItemListTag::label() const
{
    return QString(QObject::tr("%1: %2 entries"))
        .arg(m_tag->name().c_str())
        .arg(anvil::tag_cast<anvil::ListTag*>(m_tag)->size());
}

void NbtTreeItemListTag::swap(int indexA, int indexB)
{
    anvil::ListTag* listTag = dynamic_cast<anvil::ListTag*>(m_tag);
    if(listTag) {
        assert(indexA >= 0 && indexA < listTag->size());
        assert(indexB >= 0 && indexB < listTag->size());
        listTag->swap(indexA, indexB);
        m_children.swapItemsAt(indexA, indexB);
    }
}

bool NbtTreeItemListTag::canAddNbtTag(anvil::TagType type) const
{
    if(m_children.size() > 0) {
        anvil::ListTag* listTag = anvil::tag_cast<anvil::ListTag*>(m_tag);
        if(listTag && listTag->listType() == type) {
            return true;
        }
        return false;
    }
    return true;
}

void NbtTreeItemListTag::deleteChildTag(anvil::BasicTag* tag)
{
    tag_cast<anvil::ListTag*>(m_tag)->erase(tag);
}

bool NbtTreeItemListTag::canPaste() const
{
    const QMimeData* mimeData      = qApp->clipboard()->mimeData();
    const TagMimeData* tagMimeData = dynamic_cast<const TagMimeData*>(mimeData);
    if(mimeData && mimeData->hasFormat(TagMimeData::TagMimeType)) {
        anvil::ListTag* listTag = dynamic_cast<anvil::ListTag*>(m_tag);
        if(listTag) {
            if(listTag->size() == 0) {
                return true;
            }
            if(tagMimeData) {
                std::shared_ptr<anvil::BasicTag> tagData = tagMimeData->toTagData();
                if(tagData && listTag->listType() == tagData->type()) {
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
NbtTreeItemByteArrayTag::NbtTreeItemByteArrayTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemByteArrayTag::~NbtTreeItemByteArrayTag() = default;

QIcon NbtTreeItemByteArrayTag::icon() const
{
    return QIcon(":/icons/16x16/ByteArrayTag.png");
}

QString NbtTreeItemByteArrayTag::label() const
{
    anvil::ByteArrayTag* tag = anvil::tag_cast<anvil::ByteArrayTag*>(m_tag);
    QString name;
    if(!tag->name().empty()) {
        name = QString("%1: ").arg(tag->name().c_str());
    }
    name += QString("%1 Bytes").arg(tag->size());

    return name;
}

/// IntArrayTag
NbtTreeItemIntArrayTag::NbtTreeItemIntArrayTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemIntArrayTag::~NbtTreeItemIntArrayTag() = default;

QIcon NbtTreeItemIntArrayTag::icon() const
{
    return QIcon(":/icons/16x16/IntArrayTag.png");
}

QString NbtTreeItemIntArrayTag::label() const
{
    anvil::IntArrayTag* tag = anvil::tag_cast<anvil::IntArrayTag*>(m_tag);
    QString name;
    if(!tag->name().empty()) {
        name = QString("%1: ").arg(tag->name().c_str());
    }
    name += QString("%1 Integers").arg(tag->size());

    return name;
}

/// LongArrayTag
NbtTreeItemLongArrayTag::NbtTreeItemLongArrayTag(NbtTreeItemBase* parentItem, anvil::BasicTag* tag)
    : NbtTreeItemNbtTag(parentItem, tag)
{ }

NbtTreeItemLongArrayTag::~NbtTreeItemLongArrayTag() = default;

QIcon NbtTreeItemLongArrayTag::icon() const
{
    return QIcon(":/icons/16x16/LongArrayTag.png");
}

QString NbtTreeItemLongArrayTag::label() const
{
    anvil::LongArrayTag* tag = anvil::tag_cast<anvil::LongArrayTag*>(m_tag);
    QString name;
    if(!tag->name().empty()) {
        name = QString("%1: ").arg(tag->name().c_str());
    }
    name += QString("%1 Long Integers").arg(tag->size());

    return name;
}

} // namespace nbt
} // namespace minecraft
