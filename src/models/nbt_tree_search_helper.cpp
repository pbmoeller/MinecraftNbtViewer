#include "nbt_tree_search_helper.hpp"
#include "nbt_data_treemodel.hpp"
#include "treeitems/nbt_treeitem_base.hpp"
#include "treeitems/nbt_treeitem_nbttag.hpp"

namespace minecraft {
namespace nbt {

NbtTreeSearchHelper::NbtTreeSearchHelper(NbtDataTreeModel* model)
    : m_model(model)
{ }

void NbtTreeSearchHelper::reset(const SearchCriteria& criteria, const QModelIndex& startIndex)
{
    m_searchCriteria = criteria;
    m_lastFindIndex  = startIndex;
}

QModelIndex NbtTreeSearchHelper::find(const QModelIndex& startIndex, const SearchCriteria& criteria)
{
    qDebug() << "NbtTreeSearchHelper::find() called";
    m_searchCriteria = criteria;

    m_caseSensitivity = m_searchCriteria.matchFlags.testFlag(Qt::MatchCaseSensitive)
                          ? Qt::CaseSensitive
                          : Qt::CaseInsensitive;

    return findNext(startIndex);
}

QModelIndex NbtTreeSearchHelper::findNext(const QModelIndex& startIndex)
{
    qDebug() << "NbtTreeSearchHelper::findNext() called";
    m_lastFindIndex = startIndex;
    return find(true);
}

QModelIndex NbtTreeSearchHelper::findPrevious(const QModelIndex& startIndex)
{
    qDebug() << "NbtTreeSearchHelper::findPrevious() called";
    m_lastFindIndex = startIndex;
    return find(false);
}

QModelIndex NbtTreeSearchHelper::find(bool forward)
{
    // The last index is invalid. We can not continue search.
    if(!m_lastFindIndex.isValid()) {
        return {};
    }

    // If the user selected backwards search direction in the dialog,
    // then we need to switch switch the direction of next and previous.
    forward = (m_searchCriteria.direction == SearchDirection::Forward ? forward : !forward);

    QModelIndex nextIndex;
    if(forward) {
        nextIndex = nextIndexFwdDfs(m_lastFindIndex);
        while(nextIndex.isValid() && !matchesCriteria(nextIndex)) {
            nextIndex = nextIndexFwdDfs(nextIndex);
        }
    } else {
        nextIndex = prevIndexFwdDfs(m_lastFindIndex);
        while(nextIndex.isValid() && !matchesCriteria(nextIndex)) {
            nextIndex = prevIndexFwdDfs(nextIndex);
        }
    }

    if(nextIndex.isValid()) {
        m_lastFindIndex = nextIndex;
        // qDebug() << "Found item: " << m_lastFindItem->label();
        return nextIndex;
    }

    qDebug() << "No more items found matching criteria";
    return {};
}

QModelIndex NbtTreeSearchHelper::nextIndexFwdDfs(const QModelIndex& current)
{
    // Has children? => go to first child
    if(m_model->rowCount(current) > 0) {
        return m_model->index(0, 0, current);
    }

    // Try to get next sibling. If the item itself is last child of the parent,
    // then go up to the parent and continue to go to next sibling.
    QModelIndex tmp = current;
    while(tmp.isValid()) {
        QModelIndex parent = tmp.parent();
        int row            = tmp.row();
        int siblings       = m_model->rowCount(parent);
        if(row + 1 < siblings) {
            return m_model->index(row + 1, 0, parent);
        }
        tmp = parent;
    }

    // Return empty index if no more parents/siblings/children can be found.
    return {};
}

QModelIndex NbtTreeSearchHelper::prevIndexFwdDfs(const QModelIndex& current)
{
    QModelIndex parent = current.parent();
    int row            = current.row();

    // If the current Index has at least one sibling before itself in the list.
    // Go to the sibling and as deep as possible in the hierarchy.
    if(row > 0) {
        QModelIndex previousSibling = m_model->index(row - 1, 0, parent);

        while(m_model->rowCount(previousSibling) > 0) {
            int lastChildRow = m_model->rowCount(previousSibling) - 1;
            previousSibling  = m_model->index(lastChildRow, 0, previousSibling);
        }
        return previousSibling;
    }

    // Return parent if no previous siblings are available.
    return parent;
}

bool NbtTreeSearchHelper::matchesCriteria(const QModelIndex& index) const
{
    auto* treeItem = m_model->fromIndex(index);
    if(!treeItem) {
        return false;
    }

    // Flag if at least one check has been made. We can not let the match succeed, if no check has
    // been made.
    bool atLeastOneChecked = false;

    // Check for the name
    if(m_searchCriteria.isFindName && !m_searchCriteria.name.isEmpty()) {
        atLeastOneChecked = true;
        if(QString::compare(treeItem->name(), m_searchCriteria.name, m_caseSensitivity) != 0) {
            return false;
        }
    }

    // Check for the type
    if(m_searchCriteria.isFindType && treeItem->isNbtTag()) {
        atLeastOneChecked = true;
        if(static_cast<NbtTreeItemNbtTag*>(treeItem)->tagType() != m_searchCriteria.type) {
            return false;
        }
    }

    return atLeastOneChecked;
}

} // namespace nbt
} // namespace minecraft
