#ifndef AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_BASE_HPP
#define AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_BASE_HPP

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

    virtual bool canRename() const;
    virtual void rename(const QString &name);

    virtual bool canDelete() const;

    virtual bool canAddNbtTag(amc::TagType type) const;

    virtual bool canFetchMore() const;
    virtual void fetchMore();

protected:
    NbtTreeItemBase *m_parent;
    QVector<NbtTreeItemBase*> m_children;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_BASE_HPP