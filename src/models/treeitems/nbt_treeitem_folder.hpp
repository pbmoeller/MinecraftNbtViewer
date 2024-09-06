#ifndef MINECRAFTBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_FOLDER_HPP
#define MINECRAFTBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_FOLDER_HPP

// MinecraftNbtViewer
#include "nbt_treeitem_base.hpp"

// Qt
#include <QString>

namespace minecraft {
namespace nbt {

class NbtTreeItemFolder : public NbtTreeItemBase
{
public:
    NbtTreeItemFolder(NbtTreeItemBase *parentItem,
                      const QString &folderName,
                      const QString &pathToFolder);
    virtual ~NbtTreeItemFolder();

    virtual QIcon getIcon() const override;
    virtual QString getLabel() const override;

    virtual bool canRefresh() const override;

    virtual bool canAddNbtFile() const override;

    virtual bool canOpenInExplorer() const override;
    virtual QString getPath() const override;

    virtual bool canFetchMore() const override;
    virtual void fetchMore() override;

protected:
    void readKnownFilesInDirectory(NbtTreeItemBase *parent,
                                   const QString &directory);

private:
    bool m_canFetchData;
    QString m_folderName;
    QString m_pathToFolder;
};

class NbtTreeItemFolderWorld : public NbtTreeItemFolder
{
public:
    NbtTreeItemFolderWorld(NbtTreeItemBase *parentItem,
                           const QString &folderName,
                           const QString &pathToFolder);
    virtual ~NbtTreeItemFolderWorld();

    virtual QIcon getIcon() const override;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_FOLDER_HPP
