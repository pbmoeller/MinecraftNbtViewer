#ifndef AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTTAG_HPP
#define AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTTAG_HPP

// AwesomeNbtViewer
#include "nbt_treeitem_base.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/tags/abstract_tag.hpp>

namespace anv
{

class NbtTreeItemNbtTag : public NbtTreeItemBase
{
public:
    NbtTreeItemNbtTag(NbtTreeItemBase *parentItem = nullptr,
                      amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemNbtTag();

    virtual QIcon getIcon() const override;
    virtual QString getName() const override;
    virtual QString getLabel() const override;
    amc::TagType getTagType() const;
    amc::AbstractTag* getTag();

    virtual bool canRename() const;
    virtual void rename(const QString &name) override;

    void sort();

protected:
    amc::AbstractTag *m_tag;
};

class NbtTreeItemByteTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemByteTag(NbtTreeItemBase *parentItem = nullptr,
                       amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemByteTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemShortTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemShortTag(NbtTreeItemBase *parentItem = nullptr,
                        amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemShortTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemIntTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemIntTag(NbtTreeItemBase *parentItem = nullptr,
                      amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemIntTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemLongTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemLongTag(NbtTreeItemBase *parentItem = nullptr,
                       amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemLongTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemFloatTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemFloatTag(NbtTreeItemBase *parentItem = nullptr,
                        amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemFloatTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemDoubleTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemDoubleTag(NbtTreeItemBase *parentItem = nullptr,
                         amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemDoubleTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemStringTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemStringTag(NbtTreeItemBase *parentItem = nullptr,
                         amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemStringTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemCompoundTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemCompoundTag(NbtTreeItemBase *parentItem = nullptr,
                           amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemCompoundTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual bool canAddNbtTag(amc::TagType type) const override;
};

class NbtTreeItemListTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemListTag(NbtTreeItemBase *parentItem = nullptr,
                       amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemListTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual bool canAddNbtTag(amc::TagType type) const override;
};

class NbtTreeItemByteArrayTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemByteArrayTag(NbtTreeItemBase *parentItem = nullptr,
                            amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemByteArrayTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemIntArrayTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemIntArrayTag(NbtTreeItemBase *parentItem = nullptr,
                           amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemIntArrayTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemLongArrayTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemLongArrayTag(NbtTreeItemBase *parentItem = nullptr,
                            amc::AbstractTag *tag = nullptr);
    virtual ~NbtTreeItemLongArrayTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTTAG_HPP