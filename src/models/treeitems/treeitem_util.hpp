#ifndef MINECRAFTNBTVIEWER_MODELS_TREEITEMS_TREEITEM_UTIL_HPP
#define MINECRAFTNBTVIEWER_MODELS_TREEITEMS_TREEITEM_UTIL_HPP

// MinecraftNbtViewer
#include "nbt_treeitem_nbttag.hpp"

// cpp-anvil
#include <cpp-anvil/nbt/basic_tag.hpp>

namespace minecraft {
namespace nbt {

void addNbtChild(NbtTreeItemBase* parentItem, anvil::BasicTag* tag);

bool treeItemNbtTagCompare(NbtTreeItemBase* left, NbtTreeItemBase* right);

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_MODELS_TREEITEMS_TREEITEM_UTIL_HPP
