// AwesomeNbtViewer
#include "nbt_data_treemodel.hpp"
#include "treeitems/nbt_treeitem_base.hpp"
#include "treeitems/nbt_treeitem_folder.hpp"
#include "util/minecraft_util.hpp"

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

void NbtDataTreeModel::addNbtTag(const QModelIndex& parent, amc::TagType type, const QString& name)
{
    qDebug() << "Want to add: " << name << " of Type: " << static_cast<int>(type);
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
                QString s = item->getName();
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