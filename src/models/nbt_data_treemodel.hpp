#ifndef AWESOMENBTVIEWER_MODELS_NBT_DATA_TREEMODEL_HPP
#define AWESOMENBTVIEWER_MODELS_NBT_DATA_TREEMODEL_HPP

// Qt
#include <QAbstractItemModel>

namespace anv
{

class NbtDataTreeNode;

class NbtDataTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit NbtDataTreeModel(QObject *parent = nullptr);
    virtual ~NbtDataTreeModel();

    void load(const QString &directory);

    // Reimplemented functions of QAbstractItemModel
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual void fetchMore(const QModelIndex &parent) override;
    virtual bool canFetchMore(const QModelIndex& parent) const override;

private:
    NbtDataTreeNode *m_rootItem;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_NBT_DATA_TREEMODEL_HPP