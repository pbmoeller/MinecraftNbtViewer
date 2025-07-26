// MinecraftNbtViewer
#include "nbt_treeitem_base.hpp"
#include "models/nbt_data_treemodel.hpp"
#include "nbt_treeitem_folder.hpp"

// Qt
#include <QDebug>

namespace minecraft {
namespace nbt {

NbtTreeItemBase::NbtTreeItemBase(NbtTreeItemBase* parentItem)
    : m_parent(parentItem)
{
    if(m_parent) {
        m_parent->children().append(this);
    }
}

NbtTreeItemBase::~NbtTreeItemBase()
{
    if(m_parent) {
        m_parent->children().removeOne(this);
    }
    clear();
}

NbtTreeItemBase*& NbtTreeItemBase::parent()
{
    return m_parent;
}

QVector<NbtTreeItemBase*>& NbtTreeItemBase::children()
{
    return m_children;
}

bool NbtTreeItemBase::hasChildWithName(const QString& name) const
{
    for(const auto& child : m_children) {
        if(child->name() == name) {
            return true;
        }
    }

    return false;
}

void NbtTreeItemBase::sort()
{
    std::sort(m_children.begin(), m_children.end(),
              [](NbtTreeItemBase* left, NbtTreeItemBase* right) {
                  if(!left || !right) {
                      return false;
                  }
                  if(dynamic_cast<NbtTreeItemFolder*>(left) != nullptr
                     && dynamic_cast<NbtTreeItemFolder*>(right) == nullptr) {
                      return true;
                  }
                  if(dynamic_cast<NbtTreeItemFolder*>(right) != nullptr
                     && dynamic_cast<NbtTreeItemFolder*>(left) == nullptr) {
                      return false;
                  }
                  return left->label().toLower() < right->label().toLower();
              });
}

void NbtTreeItemBase::clear()
{
    for(auto& child : m_children) {
        child->parent() = nullptr;
        delete child;
    }
    m_children.clear();
}

QIcon NbtTreeItemBase::icon() const
{
    return {};
}

QString NbtTreeItemBase::name() const
{
    return {};
}

QString NbtTreeItemBase::label() const
{
    return {};
}

bool NbtTreeItemBase::canSave() const
{
    return false;
}

void NbtTreeItemBase::save() { }

void NbtTreeItemBase::saveAs(const QString& filename, const anvil::CompressionType compression)
{
    Q_UNUSED(filename);
    Q_UNUSED(compression);
}

bool NbtTreeItemBase::canRefresh() const
{
    return false;
}

bool NbtTreeItemBase::canRename() const
{
    return false;
}

void NbtTreeItemBase::rename(const QString& name)
{
    Q_UNUSED(name);
}

void NbtTreeItemBase::openRenameDialog(NbtDataTreeModel* model)
{
    Q_UNUSED(model);
}

bool NbtTreeItemBase::canEdit() const
{
    return false;
}

void NbtTreeItemBase::openEditDialog(NbtDataTreeModel* model)
{
    Q_UNUSED(model);
}

bool NbtTreeItemBase::canDelete() const
{
    return false;
}

bool NbtTreeItemBase::canCut() const
{
    return false;
}

void NbtTreeItemBase::cut() { }

bool NbtTreeItemBase::canCopy() const
{
    return false;
}

void NbtTreeItemBase::copy() { }

bool NbtTreeItemBase::canPaste() const
{
    return false;
}

void NbtTreeItemBase::paste() { }

bool NbtTreeItemBase::canMoveUp() const
{
    return false;
}

bool NbtTreeItemBase::canMoveDown() const
{
    return false;
}

void NbtTreeItemBase::swap(int indexA, int indexB)
{
    Q_UNUSED(indexA);
    Q_UNUSED(indexB);
}

bool NbtTreeItemBase::canAddNbtFile() const
{
    return false;
}

bool NbtTreeItemBase::canAddNbtTag(anvil::TagType type) const
{
    Q_UNUSED(type);
    return false;
}

bool NbtTreeItemBase::canOpenInExplorer() const
{
    return false;
}

QString NbtTreeItemBase::path() const
{
    return {};
}

bool NbtTreeItemBase::canFetchMore() const
{
    return false;
}

void NbtTreeItemBase::fetchMore() { }

NbtTreeItemBase* NbtTreeItemBase::markItemDirty()
{
    return nullptr;
}

} // namespace nbt
} // namespace minecraft
