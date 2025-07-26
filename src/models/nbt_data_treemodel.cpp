// MinecraftNbtViewer
#include "nbt_data_treemodel.hpp"
#include "treeitems/nbt_treeitem_base.hpp"
#include "treeitems/nbt_treeitem_folder.hpp"
#include "treeitems/nbt_treeitem_nbtfile.hpp"
#include "treeitems/nbt_treeitem_nbttag.hpp"
#include "treeitems/treeitem_util.hpp"
#include "util/minecraft_util.hpp"

// cpp-anvil
#include <cpp-anvil/nbt.hpp>

// Qt
#include <QDir>

namespace minecraft {
namespace nbt {

NbtDataTreeModel::NbtDataTreeModel(QObject* parent)
    : QAbstractItemModel(parent)
    , m_rootItem(nullptr)
{ }

NbtDataTreeModel::~NbtDataTreeModel()
{
    clear();
}

void NbtDataTreeModel::clear()
{
    if(m_rootItem) {
        delete m_rootItem;
    }
    m_rootItem = nullptr;
}

void NbtDataTreeModel::load(const QString& directory)
{
    beginResetModel();
    clear();
    m_rootItem = new NbtTreeItemBase(nullptr);

    // Get directory and path names
    QDir dir(directory);
    QString directoryName = dir.dirName();
    dir.cdUp();
    QString pathToDirectory = dir.absolutePath();

    // Create new directory depending on its type
    if(isMinecraftWorldDirectory(directory)) {
        new NbtTreeItemFolderWorld(m_rootItem, directoryName, pathToDirectory);
    } else {
        new NbtTreeItemFolder(m_rootItem, directoryName, pathToDirectory);
    }

    // m_rootItem->sort();
    endResetModel();
}

void NbtDataTreeModel::save(const QModelIndex& index)
{
    NbtTreeItemBase* treeItem = fromIndex(index);
    if(treeItem) {
        treeItem->save();
        m_dirtyItems.remove(treeItem);
        emit modified();
    }
}

void NbtDataTreeModel::saveAs(const QModelIndex& index, const QString& saveFilename,
                              const anvil::CompressionType compression)
{
    NbtTreeItemBase* treeItem = fromIndex(index);
    if(treeItem) {
        treeItem->saveAs(saveFilename, compression);
        m_dirtyItems.remove(treeItem);
        emit modified();
    }
}

void NbtDataTreeModel::saveAll()
{
    for(NbtTreeItemBase* treeItem : m_dirtyItems) {
        treeItem->save();
    }
    m_dirtyItems.clear();

    emit modified();
}

void NbtDataTreeModel::refresh(const QModelIndex& index)
{
    NbtTreeItemBase* treeItem = fromIndex(index);

    clearDirtyItems(treeItem);
    emit modified();

    qsizetype size = treeItem->children().size();
    if(size > 0) {
        beginRemoveRows(index, 0, size - 1);
        treeItem->clear();
        endRemoveRows();
    }

    treeItem->fetchMore();
}

bool NbtDataTreeModel::isModified() const
{
    return !m_dirtyItems.isEmpty();
}

bool NbtDataTreeModel::isDirty(const QModelIndex& index)
{
    NbtTreeItemBase* treeItem = fromIndex(index);
    return treeItem && m_dirtyItems.contains(treeItem);
}

NbtTreeItemBase* NbtDataTreeModel::fromIndex(const QModelIndex& index) const
{
    if(!index.isValid()) {
        return m_rootItem;
    }
    return static_cast<NbtTreeItemBase*>(index.internalPointer());
}

QModelIndex NbtDataTreeModel::toIndex(NbtTreeItemBase* item, int column) const
{
    if(!item || item == m_rootItem) {
        return {};
    }
    NbtTreeItemBase* parent = item->parent();
    if(!parent) {
        parent = m_rootItem;
    }
    int row = parent->children().lastIndexOf(item);
    if(row == -1) {
        return {};
    }
    return createIndex(row, column, item);
}

void NbtDataTreeModel::addNbtTag(const QModelIndex& index, NbtTreeItemNbtTag* item,
                                 anvil::TagType type, const QString& name, int size)
{
    if(!anvil::isContainerTag(item->tagType())) {
        return;
    }

    // TODO: Maybe check  the list type in advance, so that it is not created if it cannot be added.

    std::unique_ptr<anvil::BasicTag> newTag;
    switch(type) {
        case anvil::TagType::Byte:
            newTag = std::make_unique<anvil::ByteTag>(name.toStdString());
            break;
        case anvil::TagType::Short:
            newTag = std::make_unique<anvil::ShortTag>(name.toStdString());
            break;
        case anvil::TagType::Int:
            newTag = std::make_unique<anvil::IntTag>(name.toStdString());
            break;
        case anvil::TagType::Long:
            newTag = std::make_unique<anvil::LongTag>(name.toStdString());
            break;
        case anvil::TagType::Float:
            newTag = std::make_unique<anvil::FloatTag>(name.toStdString());
            break;
        case anvil::TagType::Double:
            newTag = std::make_unique<anvil::DoubleTag>(name.toStdString());
            break;
        case anvil::TagType::ByteArray:
            newTag = std::make_unique<anvil::ByteArrayTag>(name.toStdString(),
                                                           std::vector<int8_t>(size, 0));
            break;
        case anvil::TagType::String:
            newTag = std::make_unique<anvil::StringTag>(name.toStdString());
            break;
        case anvil::TagType::List:
            newTag = std::make_unique<anvil::ListTag>(name.toStdString());
            break;
        case anvil::TagType::Compound:
            newTag = std::make_unique<anvil::CompoundTag>(name.toStdString());
            break;
        case anvil::TagType::IntArray:
            newTag = std::make_unique<anvil::IntArrayTag>(name.toStdString(),
                                                          std::vector<int32_t>(size, 0));
            break;
        case anvil::TagType::LongArray:
            newTag = std::make_unique<anvil::LongArrayTag>(name.toStdString(),
                                                           std::vector<int64_t>(size, 0));
            break;
        default:
            break;
    }
    if(!newTag) {
        return;
    }

    // Add to model
    NbtTreeItemBase* treeItem = fromIndex(index);
    int pos                   = treeItem->children().size();
    beginInsertRows(index, pos, pos);
    addNbtChild(treeItem, newTag.get());
    endInsertRows();

    markItemDirty(treeItem);

    if(item->tagType() == anvil::TagType::List) {
        anvil::tag_cast<anvil::ListTag*>(item->tag())->push_back(std::move(newTag));
    } else if(item->tagType() == anvil::TagType::Compound) {
        anvil::tag_cast<anvil::CompoundTag*>(item->tag())->push_back(std::move(newTag));
    }
}

void NbtDataTreeModel::addNewNbtFile(const QModelIndex& index)
{
    NbtTreeItemFolder* treeitemFolder = dynamic_cast<NbtTreeItemFolder*>(fromIndex(index));
    if(!treeitemFolder) {
        return;
    }

    int pos = treeitemFolder->children().size();
    beginInsertRows(index, pos, pos);
    NbtTreeItemNbtFile* nbtFile =
        NbtTreeItemNbtFile::createNewNbtFile(treeitemFolder, treeitemFolder->path());
    endInsertRows();

    markItemDirty(nbtFile);
}

void NbtDataTreeModel::renameTag(const QModelIndex& index)
{
    fromIndex(index)->openRenameDialog(this);
}

void NbtDataTreeModel::editTag(const QModelIndex& index)
{
    fromIndex(index)->openEditDialog(this);
}

void NbtDataTreeModel::deleteTag(const QModelIndex& index)
{
    NbtTreeItemNbtTag* treeItem       = dynamic_cast<NbtTreeItemNbtTag*>(fromIndex(index));
    NbtTreeItemNbtTag* parentTreeItem = dynamic_cast<NbtTreeItemNbtTag*>(treeItem->parent());
    if(!treeItem) {
        return;
    }

    beginRemoveRows(index.parent(), index.row(), index.row());
    // If the parent exists, let it delete the NBT item, since it keeps the ownership.
    if(parentTreeItem) {
        parentTreeItem->deleteChildTag(treeItem->tag());
    } else {
        // TODO: Check if this can happen, this should only happen if the parent is NbtFile
        // See possible Bug with missing root Tag.
        treeItem->deleteTag();
    }

    delete treeItem;
    endRemoveRows();

    markItemDirty(parentTreeItem);
}

void NbtDataTreeModel::cutTag(const QModelIndex& index)
{
    NbtTreeItemBase* item       = fromIndex(index);
    NbtTreeItemBase* parentItem = item->parent();

    if(item && item->canCut() && parentItem) {
        beginRemoveRows(index.parent(), index.row(), index.row());
        item->cut();
        endRemoveRows();

        markItemDirty(parentItem);
    }
}

void NbtDataTreeModel::copyTag(const QModelIndex& index)
{
    NbtTreeItemBase* item = fromIndex(index);
    if(item && item->canCopy()) {
        item->copy();
    }
}

void NbtDataTreeModel::pasteTag(const QModelIndex& index)
{
    NbtTreeItemBase* item = fromIndex(index);
    qsizetype pos         = item->children().size();

    if(item && item->canPaste()) {
        beginInsertRows(index, pos, pos);
        item->paste();
        endInsertRows();

        markItemDirty(item);
    }
}

void NbtDataTreeModel::moveUp(const QModelIndex& index)
{
    int sourceRowIndex       = index.row();
    int targetRowIndex       = index.row() - 1;
    QModelIndex sourceParent = index.parent();
    QModelIndex targetParent = index.parent();

    beginMoveRows(sourceParent, sourceRowIndex, sourceRowIndex, targetParent, targetRowIndex);
    NbtTreeItemBase* item          = fromIndex(index);
    NbtTreeItemListTag* parentItem = dynamic_cast<NbtTreeItemListTag*>(item->parent());
    parentItem->swap(sourceRowIndex, targetRowIndex);
    endMoveRows();

    markItemDirty(parentItem);
}

void NbtDataTreeModel::moveDown(const QModelIndex& index)
{
    int sourceRowIndex       = index.row() + 1;
    int targetRowIndex       = index.row();
    QModelIndex sourceParent = index.parent();
    QModelIndex targetParent = index.parent();

    beginMoveRows(sourceParent, sourceRowIndex, sourceRowIndex, targetParent, targetRowIndex);
    NbtTreeItemBase* item          = fromIndex(index);
    NbtTreeItemListTag* parentItem = dynamic_cast<NbtTreeItemListTag*>(item->parent());
    parentItem->swap(sourceRowIndex, targetRowIndex);
    endMoveRows();

    markItemDirty(parentItem);
}

void NbtDataTreeModel::itemChanged(NbtTreeItemBase* item)
{
    QModelIndex index = toIndex(item);
    markItemDirty(item);
    emit dataChanged(index, index);
}

void NbtDataTreeModel::markItemDirty(NbtTreeItemBase* treeItem)
{
    if(treeItem) {
        NbtTreeItemBase* dirtyItem = treeItem->markItemDirty();
        if(dirtyItem) {
            m_dirtyItems.insert(dirtyItem);
            emit modified();
        }
    }
}

void NbtDataTreeModel::clearDirtyItems(NbtTreeItemBase* treeItem)
{
    if(m_dirtyItems.contains(treeItem)) {
        m_dirtyItems.remove(treeItem);
    }
    for(NbtTreeItemBase* child : treeItem->children()) {
        clearDirtyItems(child);
    }
}

QVariant NbtDataTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();
}

QVariant NbtDataTreeModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }

    NbtTreeItemBase* item = fromIndex(index);
    if(item) {
        switch(role) {
            case Qt::DisplayRole:
            {
                QString s = item->label();
                if(m_dirtyItems.contains(item)) {
                    s = "* " + s;
                }
                return s;
            }
            case Qt::DecorationRole:
                return item->icon();
        }
    }
    return QVariant();
}

QModelIndex NbtDataTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if(row < 0 || column < 0 || column >= ColumnCount) {
        return QModelIndex();
    }

    NbtTreeItemBase* parentItem;
    if(parent.isValid()) {
        parentItem = fromIndex(parent);
    } else {
        parentItem = m_rootItem;
    }

    if(parentItem && row < parentItem->children().count()) {
        NbtTreeItemBase* childItem = parentItem->children().at(row);
        if(!childItem) {
            return QModelIndex();
        }
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex NbtDataTreeModel::parent(const QModelIndex& index) const
{
    if(!index.isValid()) {
        return QModelIndex();
    }

    NbtTreeItemBase* childItem = static_cast<NbtTreeItemBase*>(index.internalPointer());
    if(!childItem || childItem == m_rootItem) {
        return QModelIndex();
    }

    return toIndex(childItem->parent(), 0);
}

int NbtDataTreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

int NbtDataTreeModel::rowCount(const QModelIndex& parent) const
{
    if(!m_rootItem) {
        return 0;
    }

    if(!parent.isValid()) {
        return m_rootItem->children().count();
    }

    NbtTreeItemBase* parentItem = fromIndex(parent);
    if(!parentItem) {
        return 0;
    }
    return parentItem->children().count();
}

bool NbtDataTreeModel::hasChildren(const QModelIndex& parent) const
{
    if(!m_rootItem) {
        return false;
    }
    if(!parent.isValid()) {
        return m_rootItem->children().count() > 0;
    }
    NbtTreeItemBase* item = fromIndex(parent);
    if(!item) {
        return false;
    }
    return (item->children().count() > 0 || item->canFetchMore());
}

void NbtDataTreeModel::fetchMore(const QModelIndex& parent)
{
    NbtTreeItemBase* parentItem = fromIndex(parent);
    if(parentItem) {
        parentItem->fetchMore();
        beginInsertRows(parent, 0, parentItem->children().size());
        endInsertRows();
    }
}

bool NbtDataTreeModel::canFetchMore(const QModelIndex& parent) const
{
    NbtTreeItemBase* parentItem = fromIndex(parent);
    if(parentItem) {
        return parentItem->canFetchMore();
    }
    return true;
}

} // namespace nbt
} // namespace minecraft
