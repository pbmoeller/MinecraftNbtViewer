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

    m_matchType       = criteria.matchFlags & 0x0F;
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

    bool wrap = m_searchCriteria.matchFlags & Qt::MatchWrap;

    // If the user selected backwards search direction in the dialog,
    // then we need to switch switch the direction of next and previous.
    forward = (m_searchCriteria.direction == SearchDirection::Forward ? forward : !forward);

    const QModelIndex startIndex = m_lastFindIndex;
    QModelIndex nextIndex = forward ? nextIndexDfs(m_lastFindIndex) : prevIndexDfs(m_lastFindIndex);
    while(nextIndex.isValid() && nextIndex != startIndex) {
        if(matchesCriteria(nextIndex)) {
            m_lastFindIndex = nextIndex;
            return nextIndex;
        }

        nextIndex = forward ? nextIndexDfs(nextIndex) : prevIndexDfs(nextIndex);

        if(wrap && !nextIndex.isValid()) {
            nextIndex = forward ? m_model->index(0, 0) : lastIndexDfs();
        }
    }

    if(wrap && matchesCriteria(startIndex)) {
        // If the start index matches the criteria, we return it.
        m_lastFindIndex = startIndex;
        return startIndex;
    }

    qDebug() << "No more items found matching criteria";
    return {};
}

QModelIndex NbtTreeSearchHelper::nextIndexDfs(const QModelIndex& current)
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

QModelIndex NbtTreeSearchHelper::prevIndexDfs(const QModelIndex& current)
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

QModelIndex NbtTreeSearchHelper::lastIndexDfs()
{
    // Start from the root item. Double Check that it is vaid.
    QModelIndex current = m_model->index(0, 0, QModelIndex());
    if(!current.isValid()) {
        return {};
    }

    while(true) {
        int rowCount = m_model->rowCount(current);
        if(rowCount > 0) {
            // If the current item has children, go to the last child.
            current = m_model->index(rowCount - 1, 0, current);
        } else {
            // If no children are available, return the current index.
            return current;
        }
    }
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
        if(!matchString(treeItem->name(), m_searchCriteria.name, m_matchType, m_caseSensitivity)) {
            return false;
        }
    }

    // Check for the value
    if(m_searchCriteria.isFindValue && !m_searchCriteria.value.isEmpty()) {
        atLeastOneChecked = true;
        if(!matchString(treeItem->value(), m_searchCriteria.value, m_matchType,
                        m_caseSensitivity)) {
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

bool NbtTreeSearchHelper::matchString(const QString& sourceString, const QString& searchString,
                                      uint matchType, Qt::CaseSensitivity caseSensitivity) const
{
    switch(matchType) {
        case Qt::MatchExactly:
            if(QString::compare(sourceString, searchString, caseSensitivity) == 0) {
                return true;
            }
            break;
        case Qt::MatchStartsWith:
            if(sourceString.startsWith(searchString, caseSensitivity)) {
                return true;
            }
            break;
        case Qt::MatchEndsWith:
            if(sourceString.endsWith(searchString, caseSensitivity)) {
                return true;
            }
            break;
        case Qt::MatchContains:
        default:
            if(sourceString.contains(searchString, caseSensitivity)) {
                return true;
            }
            break;
    }
    return false;
}

} // namespace nbt
} // namespace minecraft
