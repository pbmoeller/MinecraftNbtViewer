#ifndef MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTTAG_HPP
#define MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTTAG_HPP

// MinecraftNbtViewer
#include "nbt_treeitem_base.hpp"

// cpp-anvil
#include <cpp-anvil/nbt.hpp>

namespace minecraft {
namespace nbt {

class NbtTreeItemNbtTag : public NbtTreeItemBase
{
public:
    NbtTreeItemNbtTag(NbtTreeItemBase *parentItem = nullptr,
                      anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemNbtTag();

    virtual QIcon getIcon() const override;
    virtual QString getName() const override;
    virtual QString getLabel() const override;
    anvil::TagType getTagType() const;
    anvil::BasicTag* getTag();

    virtual bool canRename() const;
    virtual void rename(const QString &name) override;
    virtual void openRenameDialog(NbtDataTreeModel *model) override;

    virtual bool canEdit() const override;
    virtual void openEditDialog(NbtDataTreeModel *model) override;

    virtual bool canDelete() const override;
    virtual void deleteTag();
    virtual void deleteChildTag(anvil::BasicTag *tag);

    virtual bool canCut() const override;
    virtual void cut() override;
    virtual bool canCopy() const override;
    virtual void copy() override;

    virtual bool canMoveUp() const override;
    virtual bool canMoveDown() const override;

    void sort();

    virtual NbtTreeItemBase* markItemDirty() override;

protected:
    anvil::BasicTag *m_tag;
};

class NbtTreeItemByteTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemByteTag(NbtTreeItemBase *parentItem = nullptr,
                       anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemByteTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemShortTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemShortTag(NbtTreeItemBase *parentItem = nullptr,
                        anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemShortTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemIntTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemIntTag(NbtTreeItemBase *parentItem = nullptr,
                      anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemIntTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemLongTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemLongTag(NbtTreeItemBase *parentItem = nullptr,
                       anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemLongTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemFloatTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemFloatTag(NbtTreeItemBase *parentItem = nullptr,
                        anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemFloatTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemDoubleTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemDoubleTag(NbtTreeItemBase *parentItem = nullptr,
                         anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemDoubleTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemStringTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemStringTag(NbtTreeItemBase *parentItem = nullptr,
                         anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemStringTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemCompoundTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemCompoundTag(NbtTreeItemBase *parentItem = nullptr,
                           anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemCompoundTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual bool canAddNbtTag(anvil::TagType type) const override;
    virtual void deleteChildTag(anvil::BasicTag *tag) override;

    virtual bool canPaste() const override;
    virtual void paste() override;
};

class NbtTreeItemListTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemListTag(NbtTreeItemBase *parentItem = nullptr,
                       anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemListTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual void swap(int indexA, int indexB) override;

    virtual bool canAddNbtTag(anvil::TagType type) const override;
    virtual void deleteChildTag(anvil::BasicTag *tag) override;

    virtual bool canPaste() const override;
    virtual void paste() override;
};

class NbtTreeItemByteArrayTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemByteArrayTag(NbtTreeItemBase *parentItem = nullptr,
                            anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemByteArrayTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemIntArrayTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemIntArrayTag(NbtTreeItemBase *parentItem = nullptr,
                           anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemIntArrayTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

class NbtTreeItemLongArrayTag : public NbtTreeItemNbtTag
{
public:
    NbtTreeItemLongArrayTag(NbtTreeItemBase *parentItem = nullptr,
                            anvil::BasicTag *tag = nullptr);
    virtual ~NbtTreeItemLongArrayTag();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTTAG_HPP
