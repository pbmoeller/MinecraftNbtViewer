#ifndef AWESOMENBTVIEWER_MODELS_TREEITEMS_TREEITEM_UTIL_HPP
#define AWESOMENBTVIEWER_MODELS_TREEITEMS_TREEITEM_UTIL_HPP

// AwesomeNbtViewer
#include "nbt_treeitem_nbttag.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/tags/abstract_tag.hpp>

namespace anv
{

void addNbtChild(NbtTreeItemBase *parentItem,
                 amc::AbstractTag *tag);

bool treeItemNbtTagCompare(NbtTreeItemBase *left,
                           NbtTreeItemBase *right);

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_TREEITEMS_TREEITEM_UTIL_HPP