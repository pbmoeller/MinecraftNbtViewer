#ifndef AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONFILE_HPP
#define AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONFILE_HPP

// AwesomeNbtViewer
#include "nbt_treeitem_base.hpp"

// AwesomeMC
#include <AwesomeMC/anvil/region.hpp>

// Qt
#include <QString>
#include <Qicon>

// STL
#include <memory>

namespace anv
{

class NbtTreeItemRegionFile : public NbtTreeItemBase
{
public:
    NbtTreeItemRegionFile(NbtTreeItemBase *parentItem,
                          const QString &filename,
                          const QString &pathToFile);
    ~NbtTreeItemRegionFile();

    virtual QIcon getIcon() const override;
    virtual QString getName() const override;

    virtual bool canFetchMore() const override;
    virtual void fetchMore() override;

private:
    bool m_canFetchData;
    QString m_filename;
    QString m_pathToFile;
    std::unique_ptr<amc::Region> m_region;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONFILE_HPP