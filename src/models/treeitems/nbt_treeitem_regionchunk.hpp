#ifndef MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONCHUNK_HPP
#define MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONCHUNK_HPP

// MinecraftNbtViewer
#include "nbt_treeitem_base.hpp"

// cpp-anvil
#include <cpp-anvil/anvil/region.hpp>

// Qt
#include <QIcon>
#include <QString>

// STL
#include <memory>

namespace minecraft {
namespace nbt {

class NbtTreeItemRegionChunk : public NbtTreeItemBase
{
public:
    NbtTreeItemRegionChunk(NbtTreeItemBase* parentItem, anvil::Region* region, unsigned int index);
    ~NbtTreeItemRegionChunk();

    void sort();

    virtual QIcon icon() const override;
    virtual QString label() const override;

    virtual bool canFetchMore() const override;
    virtual void fetchMore() override;

private:
    bool m_canFetchData;
    unsigned int m_index;
    anvil::Region* m_parentRegion;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONCHUNK_HPP
