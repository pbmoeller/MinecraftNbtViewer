#ifndef AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTFILE_HPP
#define AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTFILE_HPP

// AwesomeNbtViewer
#include "nbt_treeitem_base.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/tags/compound_tag.hpp>

// Qt
#include <QString>
#include <QIcon>

// STL
#include <memory>

namespace anv
{

class NbtTreeItemNbtFile : public NbtTreeItemBase
{
public:
    NbtTreeItemNbtFile(NbtTreeItemBase *parentItem,
                       const QString &filename,
                       const QString &pathToFile);
    ~NbtTreeItemNbtFile();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual bool canSave() const;
    virtual void save() override;
    virtual void saveAs(const QString &filename) override;

    virtual bool canRefresh() const override;
    
    virtual bool canOpenInExplorer() const override;
    virtual QString getPath() const override;

    virtual bool canFetchMore() const override;
    virtual void fetchMore() override;

    virtual NbtTreeItemBase* markItemDirty() override;

    static NbtTreeItemNbtFile* createNewNbtFile(NbtTreeItemBase *parentItem,
                                                const QString &pathToFile);

private:
    bool m_canFetchData;
    QString m_filename;
    QString m_pathToFile;
    std::unique_ptr<amc::CompoundTag> m_nbtRootTag;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTFILE_HPP