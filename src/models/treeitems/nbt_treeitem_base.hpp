#ifndef AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_BASE_HPP
#define AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_BASE_HPP

// AwesomeNbtViewer
#include "models/nbt_data_treemodel.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/tags/abstract_tag.hpp>

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
    virtual void saveAs();

    virtual bool canRename() const;
    virtual void rename(const QString &name);

    virtual bool canEdit() const;
    virtual void openEditDialog(NbtDataTreeModel *model);

    virtual bool canDelete() const;

    virtual bool canAddNbtFile() const;
    virtual bool canAddNbtTag(amc::TagType type) const;

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

#endif // AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_BASE_HPP