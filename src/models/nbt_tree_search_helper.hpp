#ifndef MINECRAFTVIEWER_MODELS_NBT_TREE_SEARCH_HELPER_HPP
#define MINECRAFTVIEWER_MODELS_NBT_TREE_SEARCH_HELPER_HPP

#include <cpp-anvil/nbt/types.hpp>

#include <QModelIndex>

#include <optional>

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
    std::optional<QString> name;
    std::optional<QString> value;
    std::optional<anvil::TagType> type;
    SearchDirection direction{SearchDirection::Forward};
    bool caseSensitive{false};
    bool wrapAround{false};
    bool downOnly{false};
};

class NbtTreeSearchHelper
{
public:
public:
    explicit NbtTreeSearchHelper(NbtDataTreeModel* model);

    void reset(const SearchCriteria& criteria, const QModelIndex& startIndex);

    QModelIndex find(const QModelIndex& startIndex, const SearchCriteria& criteria);
    QModelIndex findNext();
    QModelIndex findPrevious();

private:
    QModelIndex find(bool forward = true);
    QModelIndex nextIndexFwdDfs(const QModelIndex& current);
    QModelIndex prevIndexFwdDfs(const QModelIndex& current);

    bool matchesCriteria(const QModelIndex& index) const;

private:
    NbtDataTreeModel* m_model{nullptr};
    SearchCriteria m_searchCriteria;

    NbtTreeItemBase* m_lastFindItem{nullptr};
    QModelIndex m_lastFindIndex{QModelIndex()};
    qsizetype m_lastFindRow{-1};
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTVIEWER_MODELS_NBT_TREE_SEARCH_HELPER_HPP
