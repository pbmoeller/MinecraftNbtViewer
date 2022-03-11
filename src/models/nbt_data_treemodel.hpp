#ifndef AWESOMENBTVIEWER_MODELS_NBT_DATA_TREEMODEL_HPP
#define AWESOMENBTVIEWER_MODELS_NBT_DATA_TREEMODEL_HPP

// AwesomeMC
#include <AwesomeMC/nbt/tags/abstract_tag.hpp>

// Qt
#include <QAbstractItemModel>

namespace anv
{

class NbtTreeItemBase;

class NbtDataTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit NbtDataTreeModel(QObject *parent = nullptr);
    virtual ~NbtDataTreeModel();

    void clear();
    void load(const QString &directory);

    NbtTreeItemBase* fromIndex(const QModelIndex &index) const;
    QModelIndex toIndex(NbtTreeItemBase *item, int column) const;
    void addNbtTag(const QModelIndex& parent, amc::TagType type, const QString& name);

    void renameTag(const QModelIndex &index, const QString &newName);

    // Reimplemented functions of QAbstractItemModel
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual bool hasChildren(const QModelIndex& parent) const override;
    virtual void fetchMore(const QModelIndex &parent) override;
    virtual bool canFetchMore(const QModelIndex& parent) const override;

private:
    NbtTreeItemBase *m_rootItem;

    static const int ColumnCount = 1;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_NBT_DATA_TREEMODEL_HPP