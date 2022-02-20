#ifndef AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_FOLDER_HPP
#define AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_FOLDER_HPP

// AwesomeNbtViewer
#include "nbt_treeitem_base.hpp"

// Qt
#include <QString>

namespace anv
{

class NbtTreeItemFolder : public NbtTreeItemBase
{
public:
    NbtTreeItemFolder(NbtTreeItemBase *parentItem,
                      const QString &folderName,
                      const QString &pathToFolder);
    virtual ~NbtTreeItemFolder();

    virtual QIcon getIcon() const override;
    virtual QString getName() const override;

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

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_FOLDER_HPP