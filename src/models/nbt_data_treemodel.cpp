// AwesomeNbtViewer
#include "nbt_data_treemodel.hpp"
#include "treeitems/nbt_treeitem_base.hpp"
#include "treeitems/nbt_treeitem_folder.hpp"
#include "treeitems/nbt_treeitem_nbttag.hpp"
#include "treeitems/nbt_treeitem_nbtfile.hpp"
#include "treeitems/treeitem_util.hpp"
#include "util/minecraft_util.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/tags/tags.hpp>

// Qt
#include <QDir>

namespace anv
{

NbtDataTreeModel::NbtDataTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_rootItem(nullptr)
{

}

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

void NbtDataTreeModel::load(const QString &directory)
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

void NbtDataTreeModel::save(const QModelIndex &index)
{
    NbtTreeItemBase *treeItem = fromIndex(index);
    if(treeItem) {
        treeItem->save();
        m_dirtyItems.remove(treeItem);
        emit modified();
    }
}

void NbtDataTreeModel::saveAll()
{
    for(NbtTreeItemBase *treeItem : m_dirtyItems) {
        treeItem->save();
    }
    m_dirtyItems.clear();

    emit modified();
}

bool NbtDataTreeModel::isModified() const
{
    return !m_dirtyItems.isEmpty();
}

NbtTreeItemBase* NbtDataTreeModel::fromIndex(const QModelIndex &index) const
{
    if(!index.isValid()) {
        return m_rootItem;
    } 
    return static_cast<NbtTreeItemBase*>(index.internalPointer());
}

QModelIndex NbtDataTreeModel::toIndex(NbtTreeItemBase *item, int column) const
{
    if(!item || item == m_rootItem) {
        return QModelIndex();
    }
    NbtTreeItemBase *parent = item->getParent();
    if(!parent) {
        parent = m_rootItem;
    }
    int row = parent->getChildren().lastIndexOf(item);
    if(row == -1) {
        return QModelIndex();
    }
    return createIndex(row, column, item);
}

void NbtDataTreeModel::addNbtTag(const QModelIndex &index, NbtTreeItemNbtTag *item, amc::TagType type, const QString& name, int size)
{
    amc::AbstractVectorTag *vectorTag = dynamic_cast<amc::AbstractVectorTag*>(item->getTag());
    if(vectorTag == nullptr) {
        return;
    }

    amc::AbstractTag *newTag = nullptr;
    switch(type) {
        case amc::TagType::Byte:
            newTag = new amc::ByteTag(name.toStdString());
            break;
        case amc::TagType::Short:
            newTag = new amc::ShortTag(name.toStdString());
            break;
        case amc::TagType::Int:
            newTag = new amc::IntTag(name.toStdString());
            break;
        case amc::TagType::Long:
            newTag = new amc::LongTag(name.toStdString());
            break;
        case amc::TagType::Float:
            newTag = new amc::FloatTag(name.toStdString());
            break;
        case amc::TagType::Double:
            newTag = new amc::DoubleTag(name.toStdString());
            break;
        case amc::TagType::ByteArray:
            newTag = new amc::ByteArrayTag(name.toStdString(), std::vector<int8_t>(size, 0));
            break;
        case amc::TagType::String:
            newTag = new amc::StringTag(name.toStdString());
            break;
        case amc::TagType::List:
            newTag = new amc::ListTag(name.toStdString());
            break;
        case amc::TagType::Compound:
            newTag = new amc::CompoundTag(name.toStdString());
            break;
        case amc::TagType::IntArray:
            newTag = new amc::IntArrayTag(name.toStdString(), std::vector<int32_t>(size, 0));
            break;
        case amc::TagType::LongArray:
            newTag = new amc::LongArrayTag(name.toStdString(), std::vector<int64_t>(size, 0));
            break;
    }
    if(newTag == nullptr) {
        return;
    }

    if(vectorTag->getType() == amc::TagType::List) {
        tag_cast<amc::ListTag*>(vectorTag)->setListType(newTag->getType());
    }
    vectorTag->pushBack(newTag);

    // Add to model
    NbtTreeItemBase *treeItem = fromIndex(index);
    int pos = treeItem->getChildren().size();
    beginInsertRows(index, pos, pos);
    addNbtChild(treeItem, newTag);
    endInsertRows();

    markItemDirty(treeItem);
}

void NbtDataTreeModel::addNewNbtFile(const QModelIndex &index)
{
    NbtTreeItemFolder *treeitemFolder = dynamic_cast<NbtTreeItemFolder*>(fromIndex(index));
    if(!treeitemFolder) {
        return;
    }

    int pos = treeitemFolder->getChildren().size();
    beginInsertRows(index, pos, pos);
    NbtTreeItemNbtFile* nbtFile = NbtTreeItemNbtFile::createNewNbtFile(treeitemFolder, treeitemFolder->getPath());
    endInsertRows();

    markItemDirty(nbtFile);
}

void NbtDataTreeModel::renameTag(const QModelIndex &index, const QString &newName)
{
    NbtTreeItemBase *treeItem = fromIndex(index);
    treeItem->rename(newName);
    dataChanged(index, index);

    markItemDirty(treeItem);
}

void NbtDataTreeModel::editTag(const QModelIndex &index)
{
    fromIndex(index)->openEditDialog(this);
}

void NbtDataTreeModel::deleteTag(const QModelIndex &index)
{
    NbtTreeItemNbtTag *treeItem = dynamic_cast<NbtTreeItemNbtTag*>(fromIndex(index));
    NbtTreeItemNbtTag *parentTreeItem = dynamic_cast<NbtTreeItemNbtTag*>(treeItem->getParent());
    if(!treeItem) {
        return;
    }

    beginRemoveRows(index.parent(), index.row(), index.row());
    // If the parent exists, let it delete the NBT item, since it keeps the ownership.
    if(parentTreeItem) {
        parentTreeItem->deleteChildTag(treeItem->getTag());
    } else {
        // TODO: Check if this can happen, this should only happen if the parent is NbtFile
        // See possible Bug with missing root Tag.
        treeItem->deleteTag();
    }

    delete treeItem;
    endRemoveRows();

    markItemDirty(parentTreeItem);
}

void NbtDataTreeModel::itemChanged(NbtTreeItemBase* item)
{
    QModelIndex index = toIndex(item);
    markItemDirty(item);
    emit dataChanged(index, index);
}

void NbtDataTreeModel::markItemDirty(NbtTreeItemBase *treeItem)
{
    if(treeItem) {
        NbtTreeItemBase *dirtyItem = treeItem->markItemDirty();
        if(dirtyItem) {
            m_dirtyItems.insert(dirtyItem);
            emit modified();
        }
    }
}

QVariant NbtDataTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();
}

QVariant NbtDataTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }

    NbtTreeItemBase *item = fromIndex(index);
    if(item) {
        switch(role) {
            case Qt::DisplayRole:
            {
                QString s = item->getLabel();
                if(m_dirtyItems.contains(item)) {
                    s = "* " + s;
                }
                return s;
            }
            case Qt::DecorationRole:
                return item->getIcon();
        }
    }
    return QVariant();
}

QModelIndex NbtDataTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row < 0 || column < 0 || column >= ColumnCount) {
        return QModelIndex();
    }

    NbtTreeItemBase *parentItem;
    if(parent.isValid()) {
        parentItem = fromIndex(parent);
    } else {
        parentItem = m_rootItem;
    }

    if(parentItem && row < parentItem->getChildren().count()) {
        NbtTreeItemBase *childItem = parentItem->getChildren().at(row);
        if(!childItem) {
            return QModelIndex();
        }
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex NbtDataTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid()) {
        return QModelIndex();
    }

    NbtTreeItemBase *childItem = static_cast<NbtTreeItemBase*>(index.internalPointer());
    if(!childItem || childItem == m_rootItem) {
        return QModelIndex();
    }

    return toIndex(childItem->getParent(), 0);
}

int NbtDataTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

int NbtDataTreeModel::rowCount(const QModelIndex &parent) const
{
    if(!m_rootItem) {
        return 0;
    }

    if(!parent.isValid()) {
        m_rootItem->getChildren().count();
    }

    NbtTreeItemBase *parentItem = fromIndex(parent);
    if(!parentItem) {
        return 0;
    }
    return parentItem->getChildren().count();
}

bool NbtDataTreeModel::hasChildren(const QModelIndex& parent) const
{
    if(!m_rootItem) {
        return false;
    }
    if(!parent.isValid()) {
        return m_rootItem->getChildren().count() > 0;
    }
    NbtTreeItemBase *item = fromIndex(parent);
    if(!item) {
        return false;
    }
    return (item->getChildren().count() > 0 || item->canFetchMore());
}

void NbtDataTreeModel::fetchMore(const QModelIndex &parent)
{
    NbtTreeItemBase *parentItem = fromIndex(parent);
    if(parentItem) {
        parentItem->fetchMore();
        beginInsertRows(parent, 0, parentItem->getChildren().size());
        endInsertRows();
    }
}

bool NbtDataTreeModel::canFetchMore(const QModelIndex& parent) const
{
    NbtTreeItemBase *parentItem = fromIndex(parent);
    if(parentItem) {
        return parentItem->canFetchMore();
    }
    return true;
}

} // namespace anv