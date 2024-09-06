// MinecraftNbtViewer
#include "nbt_treeitem_folder.hpp"
#include "nbt_treeitem_nbtfile.hpp"
#include "nbt_treeitem_regionfile.hpp"
#include "util/minecraft_util.hpp"

// Qt
#include <QDir>
#include <QFileInfo>
#include <QString>

namespace minecraft {
namespace nbt {

const QStringList KnownFilesFilter = QStringList({
    "*.dat"
    "*.mca"
    "*.mcr"
    "*.json"
    "*.dat_old"
    "*.dat_mcr"
    "*.schematic"
});

NbtTreeItemFolder::NbtTreeItemFolder(NbtTreeItemBase *parentItem,
                                     const QString &folderName,
                                     const QString &pathToFolder)
    : NbtTreeItemBase(parentItem)
    , m_canFetchData(true)
    , m_folderName(folderName)
    , m_pathToFolder(pathToFolder)
{

}

NbtTreeItemFolder::~NbtTreeItemFolder()
{

}

QIcon NbtTreeItemFolder::getIcon() const
{
    return QIcon(":/icons/16x16/Folder.png");
}

QString NbtTreeItemFolder::getLabel() const
{
    return m_folderName;
}

bool NbtTreeItemFolder::canRefresh() const
{
    return true;
}

bool NbtTreeItemFolder::canAddNbtFile() const
{
    return true;
}

bool NbtTreeItemFolder::canOpenInExplorer() const
{
    return true;
}

QString NbtTreeItemFolder::getPath() const
{
    return m_pathToFolder + '/' + m_folderName;
}

bool NbtTreeItemFolder::canFetchMore() const
{
    return m_canFetchData;
}

void NbtTreeItemFolder::fetchMore()
{
    m_canFetchData = false;
    readKnownFilesInDirectory(this, m_pathToFolder + '/' + m_folderName);
    sort();
}

void NbtTreeItemFolder::readKnownFilesInDirectory(NbtTreeItemBase *parent,
                                                  const QString &directory)
{
    QDir dir(directory);

    // Directories
    for(QString subDirectory : dir.entryList(QStringList(), QDir::Dirs)) {
        if(subDirectory == "." || subDirectory == "..") {
            continue;
        }
        if(isMinecraftWorldDirectory(directory + '/' + subDirectory)) {
            new NbtTreeItemFolderWorld(parent, subDirectory, directory);
        } else {
            new NbtTreeItemFolder(parent, subDirectory, directory);
        }
    }

    // Files
    for(QString filename : dir.entryList(QStringList()
                                            << "*.dat"
                                            << "*.nbt"
                                            << "*.mca"
                                            << "*.mcr"
                                            << "*.json"
                                            << "*.dat_old"
                                            << "*.dat_mcr"
                                            << "*.schematic",
                                         QDir::Files)) {
        QFileInfo fileInfo(filename);
        if(fileInfo.suffix() == "dat"
            || fileInfo.suffix() == "nbt"
            || fileInfo.suffix() == "dat_old"
            || fileInfo.suffix() == "dat_mcr"
            || fileInfo.suffix() == "schematic") {
            new NbtTreeItemNbtFile(parent, fileInfo.fileName(), directory);
        } else if(fileInfo.suffix() == "mca" || fileInfo.suffix() == "mcr") {
            new NbtTreeItemRegionFile(parent, fileInfo.fileName(), directory);
        } else if(fileInfo.suffix() == "json") {
            // Add JSON File
        }
    }
}

//
// NbtTreeItemFolderWorld
//

NbtTreeItemFolderWorld::NbtTreeItemFolderWorld(NbtTreeItemBase *parentItem,
                                               const QString &folderName,
                                               const QString &pathToFolder)
    : NbtTreeItemFolder(parentItem, folderName, pathToFolder)
{

}

NbtTreeItemFolderWorld::~NbtTreeItemFolderWorld()
{

}

QIcon NbtTreeItemFolderWorld::getIcon() const
{
    return QIcon(":/icons/16x16/World.png");
}

} // namespace nbt
} // namespace minecraft
