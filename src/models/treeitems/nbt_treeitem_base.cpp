// AwesomeNbtViewer
#include "nbt_treeitem_base.hpp"
#include "nbt_treeitem_folder.hpp"

// Qt
#include <QDebug>

namespace anv
{

NbtTreeItemBase::NbtTreeItemBase(NbtTreeItemBase *parentItem)
    : m_parent(parentItem)
{
    if(m_parent) {
        m_parent->getChildren().append(this);
    }
}

NbtTreeItemBase::~NbtTreeItemBase()
{
    if(m_parent) {
        m_parent->getChildren().removeOne(this);
    }
    clear();
}

NbtTreeItemBase*& NbtTreeItemBase::getParent()
{
    return m_parent;
}

QVector<NbtTreeItemBase*>& NbtTreeItemBase::getChildren()
{
    return m_children;
}

bool NbtTreeItemBase::hasChildWithName(const QString &name) const
{
    for(int i = 0; i < m_children.size(); ++i) {
        if(m_children[i]->getName() == name) {
            return true;
        }
    }
    return false;
}

void NbtTreeItemBase::sort()
{
    std::sort(m_children.begin(),
              m_children.end(), 
              [](NbtTreeItemBase *left,
                 NbtTreeItemBase *right) {
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
        return left->getLabel().toLower() < right->getLabel().toLower();
    });
}

void NbtTreeItemBase::clear()
{
    for(qsizetype i = 0; i < m_children.size(); ++i) {
        m_children[i]->getParent() = nullptr;
        delete m_children[i];
    }
    m_children.clear();
}

QIcon NbtTreeItemBase::getIcon() const
{
    return QIcon();
}

QString NbtTreeItemBase::getName() const
{
    return QString();
}

QString NbtTreeItemBase::getLabel() const
{
    return QString();
}

bool NbtTreeItemBase::canRename() const
{
    return false;
}

void NbtTreeItemBase::rename(const QString &name)
{
    Q_UNUSED(name);
}

bool NbtTreeItemBase::canAddNbtTag(amc::TagType type) const
{
    Q_UNUSED(type);
    return false;
}

bool NbtTreeItemBase::canFetchMore() const
{
    return false;
}

void NbtTreeItemBase::fetchMore()
{

}

} // namespace anv