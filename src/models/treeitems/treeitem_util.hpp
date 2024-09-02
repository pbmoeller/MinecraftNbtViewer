#ifndef AWESOMENBTVIEWER_MODELS_TREEITEMS_TREEITEM_UTIL_HPP
#define AWESOMENBTVIEWER_MODELS_TREEITEMS_TREEITEM_UTIL_HPP

// AwesomeNbtViewer
#include "nbt_treeitem_nbttag.hpp"

// AwesomeMC
#include <cpp-anvil/nbt/basic_tag.hpp>

namespace anv
{

void addNbtChild(NbtTreeItemBase *parentItem,
                 anvil::BasicTag *tag);

bool treeItemNbtTagCompare(NbtTreeItemBase *left,
                           NbtTreeItemBase *right);

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_TREEITEMS_TREEITEM_UTIL_HPP