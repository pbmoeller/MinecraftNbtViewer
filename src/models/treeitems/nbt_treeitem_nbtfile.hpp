#ifndef MINECRAFTBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTFILE_HPP
#define MINECRAFTBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTFILE_HPP

// MinecraftNbtViewer
#include "nbt_treeitem_base.hpp"

// cpp-anvil
#include <cpp-anvil/nbt/compound_tag.hpp>
#include <cpp-anvil/util/compression.hpp>

// Qt
#include <QString>
#include <QIcon>

// STL
#include <memory>

namespace minecraft {
namespace nbt {

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
    virtual void saveAs(const QString &filename, const anvil::CompressionType compression) override;

    virtual bool canRefresh() const override;
    
    virtual bool canOpenInExplorer() const override;
    virtual QString getPath() const override;

    virtual bool canFetchMore() const override;
    virtual void fetchMore() override;

    virtual NbtTreeItemBase* markItemDirty() override;

    static NbtTreeItemNbtFile* createNewNbtFile(NbtTreeItemBase *parentItem,
                                                const QString &pathToFile);

    anvil::CompressionType getCompression() const;

private:
    bool m_canFetchData;
    QString m_filename;
    QString m_pathToFile;
    std::unique_ptr<anvil::CompoundTag> m_nbtRootTag;
    anvil::CompressionType m_compressionType;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTFILE_HPP
