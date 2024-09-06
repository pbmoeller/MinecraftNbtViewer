#ifndef MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONFILE_HPP
#define MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONFILE_HPP

// MinecraftNbtViewer
#include "nbt_treeitem_base.hpp"

// cpp-anvil
#include <cpp-anvil/anvil/region.hpp>

// Qt
#include <QString>
#include <Qicon>

// STL
#include <memory>

namespace minecraft {
namespace nbt {

class NbtTreeItemRegionFile : public NbtTreeItemBase
{
public:
    NbtTreeItemRegionFile(NbtTreeItemBase *parentItem,
                          const QString &filename,
                          const QString &pathToFile);
    ~NbtTreeItemRegionFile();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual bool canRefresh() const override;

    virtual bool canOpenInExplorer() const override;
    virtual QString getPath() const override;

    virtual bool canFetchMore() const override;
    virtual void fetchMore() override;

private:
    bool m_canFetchData;
    QString m_filename;
    QString m_pathToFile;
    std::unique_ptr<anvil::Region> m_region;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_REGIONFILE_HPP
