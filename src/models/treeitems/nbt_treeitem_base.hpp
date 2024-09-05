#ifndef MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_BASE_HPP
#define MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_BASE_HPP

// MinecraftNbtViewer
#include "models/nbt_data_treemodel.hpp"

// cpp-anvil
#include <cpp-anvil/nbt/types.hpp>
#include <cpp-anvil/util/compression.hpp>

// Qt
#include <QVector>
#include <QString>
#include <QIcon>

namespace anv
{

class NbtTreeItemBase
{
public:
    NbtTreeItemBase(NbtTreeItemBase *parentItem = nullptr);
    virtual ~NbtTreeItemBase();

    NbtTreeItemBase*& getParent();
    QVector<NbtTreeItemBase*>& getChildren();
    bool hasChildWithName(const QString &name) const;

    void sort();
    void clear();

    virtual QIcon getIcon() const;
    virtual QString getName() const;
    virtual QString getLabel() const;

    virtual bool canSave() const;
    virtual void save();
    virtual void saveAs(const QString &filename, const anvil::CompressionType compression);

    virtual bool canRefresh() const;

    virtual bool canRename() const;
    virtual void rename(const QString &name);
    virtual void openRenameDialog(NbtDataTreeModel *model);

    virtual bool canEdit() const;
    virtual void openEditDialog(NbtDataTreeModel *model);

    virtual bool canDelete() const;

    virtual bool canCut() const;
    virtual void cut();
    virtual bool canCopy() const;
    virtual void copy();
    virtual bool canPaste() const;
    virtual void paste();

    virtual bool canMoveUp() const;
    virtual bool canMoveDown() const;
    virtual void swap(int indexA, int indexB);

    virtual bool canAddNbtFile() const;
    virtual bool canAddNbtTag(anvil::TagType type) const;

    virtual bool canOpenInExplorer() const;
    virtual QString getPath() const;

    virtual bool canFetchMore() const;
    virtual void fetchMore();

    virtual NbtTreeItemBase* markItemDirty();

protected:
    NbtTreeItemBase *m_parent;
    QVector<NbtTreeItemBase*> m_children;
};

} // namespace anv

#endif // MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_BASE_HPP
