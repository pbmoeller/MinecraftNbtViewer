#ifndef MINECRAFTVIEWER_MODELS_NBT_DATA_TREEMODEL_HPP
#define MINECRAFTVIEWER_MODELS_NBT_DATA_TREEMODEL_HPP

// cpp-anvil
#include <cpp-anvil/nbt/types.hpp>
#include <cpp-anvil/util/compression.hpp>

// Qt
#include <QAbstractItemModel>
#include <QSet>

namespace minecraft {
namespace nbt {

class NbtTreeItemBase;
class NbtTreeItemNbtTag;

class NbtDataTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit NbtDataTreeModel(QObject* parent = nullptr);
    virtual ~NbtDataTreeModel();

    void clear();
    void load(const QString& directory);
    void save(const QModelIndex& index);
    void saveAs(const QModelIndex& index, const QString& saveFilename,
                const anvil::CompressionType compression);
    void saveAll();
    void refresh(const QModelIndex& index);
    bool isModified() const;
    bool isDirty(const QModelIndex& index);

    NbtTreeItemBase* fromIndex(const QModelIndex& index) const;
    QModelIndex toIndex(NbtTreeItemBase* item, int column = 0) const;
    void addNbtTag(const QModelIndex& index, NbtTreeItemNbtTag* item, anvil::TagType type,
                   const QString& name, int size = 0);
    void addNewNbtFile(const QModelIndex& index);

    void renameTag(const QModelIndex& index);
    void editTag(const QModelIndex& index);
    void deleteTag(const QModelIndex& index);

    void cutTag(const QModelIndex& index);
    void copyTag(const QModelIndex& index);
    void pasteTag(const QModelIndex& index);

    void moveUp(const QModelIndex& index);
    void moveDown(const QModelIndex& index);

    void itemChanged(NbtTreeItemBase* item);

private:
    void markItemDirty(NbtTreeItemBase* treeItem);
    void clearDirtyItems(NbtTreeItemBase* treeItem);

signals:
    void modified();

public:
    // Reimplemented functions of QAbstractItemModel
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    virtual QModelIndex index(int row, int column,
                              const QModelIndex& parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex& index) const override;

    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    virtual bool hasChildren(const QModelIndex& parent) const override;
    virtual void fetchMore(const QModelIndex& parent) override;
    virtual bool canFetchMore(const QModelIndex& parent) const override;

private:
    NbtTreeItemBase* m_rootItem;
    QSet<NbtTreeItemBase*> m_dirtyItems;

    static const int ColumnCount = 1;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTVIEWER_MODELS_NBT_DATA_TREEMODEL_HPP
