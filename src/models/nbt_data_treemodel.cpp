#include "nbt_data_treemodel.hpp"
#include "nbt_data_treenode.hpp"

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
}

void NbtDataTreeModel::load(const QString &directory)
{

}

QVariant NbtDataTreeModel::data(const QModelIndex &index, int role) const
{

}

QModelIndex NbtDataTreeModel::index(int row, int column, const QModelIndex &parent) const
{

}

QModelIndex NbtDataTreeModel::parent(const QModelIndex &index) const
{

}

int NbtDataTreeModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid()) {
        return static_cast<NbtDataTreeNode*>(parent.internalPointer())->columnCount();
    }

    return m_rootItem->columnCount();
}

int NbtDataTreeModel::rowCount(const QModelIndex &parent) const
{

}

void NbtDataTreeModel::fetchMore(const QModelIndex &parent)
{
    qDebug() << "Fetch more clicked";
}

bool NbtDataTreeModel::canFetchMore(const QModelIndex& parent) const
{

}

} // namespace anv