#ifndef MINECRAFTVIEWER_MODELS_NBT_TREE_SEARCH_HELPER_HPP
#define MINECRAFTVIEWER_MODELS_NBT_TREE_SEARCH_HELPER_HPP

#include <cpp-anvil/nbt/types.hpp>

#include <QModelIndex>

namespace minecraft {
namespace nbt {

class NbtDataTreeModel;
class NbtTreeItemBase;

enum class SearchDirection
{
    Forward,
    Backward,
};

struct SearchCriteria
{
    QString name;
    QString value;
    anvil::TagType type{anvil::TagType::Byte};
    SearchDirection direction{SearchDirection::Forward};
    Qt::MatchFlags matchFlags{Qt::MatchExactly | Qt::MatchRecursive};
    bool fetchMore{false};
    bool isFindName{false};
    bool isFindValue{false};
    bool isFindType{false};
};

class NbtTreeSearchHelper
{
public:
public:
    explicit NbtTreeSearchHelper(NbtDataTreeModel* model);

    void reset(const SearchCriteria& criteria, const QModelIndex& startIndex);

    QModelIndex find(const QModelIndex& startIndex, const SearchCriteria& criteria);
    QModelIndex findNext(const QModelIndex& startIndex);
    QModelIndex findPrevious(const QModelIndex& startIndex);

private:
    QModelIndex find(bool forward = true);
    QModelIndex nextIndexDfs(const QModelIndex& current);
    QModelIndex prevIndexDfs(const QModelIndex& current);
    QModelIndex lastIndexDfs();

    bool matchesCriteria(const QModelIndex& index) const;
    bool matchString(const QString& sourceString, const QString& searchString, uint matchType,
                      Qt::CaseSensitivity caseSensitivity) const;

private:
    NbtDataTreeModel* m_model{nullptr};
    SearchCriteria m_searchCriteria;

    NbtTreeItemBase* m_lastFindItem{nullptr};
    QModelIndex m_lastFindIndex{QModelIndex()};
    qsizetype m_lastFindRow{-1};

    // Matching variables
    Qt::CaseSensitivity m_caseSensitivity{Qt::CaseInsensitive};
    uint m_matchType{Qt::MatchExactly};
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTVIEWER_MODELS_NBT_TREE_SEARCH_HELPER_HPP
