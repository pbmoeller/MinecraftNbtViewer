#ifndef AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONCHUNK_HPP
#define AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONCHUNK_HPP

// AwesomeNbtViewer
#include "nbt_treeitem_base.hpp"

// AwesomeMC
#include <AwesomeMC/anvil/region.hpp>

// Qt
#include <QString>
#include <QIcon>

// STL
#include <memory>

namespace anv
{

class NbtTreeItemRegionChunk : public NbtTreeItemBase
{
public:
    NbtTreeItemRegionChunk(NbtTreeItemBase *parentItem,
                           amc::Region *region,
                           unsigned int index);
    ~NbtTreeItemRegionChunk();

    void sort();

    virtual QIcon getIcon() const override;
    virtual QString getName() const override;

    virtual bool canFetchMore() const override;
    virtual void fetchMore() override;

private:
    bool m_canFetchData;
    unsigned int m_index;
    amc::Region *m_parentRegion;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONCHUNK_HPP