#include "main_window.hpp"
#include "ui_main_window.h"
#include "models/nbt_data_treemodel.hpp"
#include "models/treeitems/nbt_treeitem_nbttag.hpp"
#include "rename_tag_dialog.hpp"

// Qt
#include <QCloseEvent>
#include <QFileDialog>
#include <QStandardPaths>

namespace anv
{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    this->setWindowTitle(QApplication::applicationName());

    m_nbtTreeModel = new NbtDataTreeModel();
    m_ui->nbtDataTreeView->setModel(m_nbtTreeModel);
    m_ui->nbtDataTreeView->setHeaderHidden(true);
    m_ui->nbtDataTreeView->setIconSize(QSize(16, 16));

    initConnections();

    // Load default Minecraft directory
    openMinecraftFolder();
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(userReallyWantsToQuit()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::treeviewCurrentItemChanged(const QModelIndex &current,
                                            const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    updateActions();
}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), m_currentDirectory);
    if(!filename.isEmpty()) {
        qDebug() << "Opening file: " << filename;
    }
}

void MainWindow::openFolder()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Folder"), m_currentDirectory);
    if(!directory.isEmpty()) {
        qDebug() << "Opening directory: " << directory;
        m_currentDirectory = directory;
        m_nbtTreeModel->load(m_currentDirectory);
    }
}

void MainWindow::openMinecraftFolder()
{
    QString mcDirectory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                            + "/../.minecraft/saves";
    if(QDir(mcDirectory).exists()) {
        qDebug() << "Loading Minecraft save folder: " << mcDirectory;
        m_currentDirectory = mcDirectory;
        m_nbtTreeModel->load(m_currentDirectory);
    }
}

void MainWindow::openInExplorer()
{

}

void MainWindow::save()
{

}

void MainWindow::cutTag()
{

}

void MainWindow::copyTag()
{

}

void MainWindow::pasteTag()
{

}

void MainWindow::renameTag()
{
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        QString currentName = m_nbtTreeModel->fromIndex(index)->getName();

        RenameTagDialog renameTagDialog(currentName);
        if(renameTagDialog.exec() == QDialog::Accepted) {
            QString newName = renameTagDialog.getName();
            if(newName != currentName) {
                m_nbtTreeModel->renameTag(index, newName);
            }
        }
    }
}

void MainWindow::editTag()
{

}

void MainWindow::deleteTag()
{

}

void MainWindow::addByteTag()
{
    addNbtTag(amc::TagType::Byte);
}

void MainWindow::addShortTag()
{
    addNbtTag(amc::TagType::Short);
}

void MainWindow::addIntTag()
{
    addNbtTag(amc::TagType::Int);
}

void MainWindow::addLongTag()
{
    addNbtTag(amc::TagType::Long);
}

void MainWindow::addFloatTag()
{
    addNbtTag(amc::TagType::Float);
}

void MainWindow::addDoubleTag()
{
    addNbtTag(amc::TagType::Double);
}

void MainWindow::addStringTag()
{
    addNbtTag(amc::TagType::String);
}

void MainWindow::addByteArrayTag()
{
    addNbtTag(amc::TagType::ByteArray);
}

void MainWindow::addIntArrayTag()
{
    addNbtTag(amc::TagType::IntArray);
}

void MainWindow::addLongArrayTag()
{
    addNbtTag(amc::TagType::LongArray);
}

void MainWindow::addListTag()
{
    addNbtTag(amc::TagType::List);
}

void MainWindow::addCompoundTag()
{
    addNbtTag(amc::TagType::Compound);
}

void MainWindow::initConnections()
{
    /// General UI
    connect(m_ui->nbtDataTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::treeviewCurrentItemChanged);

    /// Actions
    // File Menu
    connect(m_ui->actionOpen_File,                  &QAction::triggered, this, &MainWindow::openFile);
    connect(m_ui->actionOpen_Folder,                &QAction::triggered, this, &MainWindow::openFolder);
    connect(m_ui->actionOpen_Minecraft_Save_Folder, &QAction::triggered, this, &MainWindow::openMinecraftFolder);
    connect(m_ui->actionOpen_in_Explorer,           &QAction::triggered, this, &MainWindow::openInExplorer);
    connect(m_ui->actionSave,                       &QAction::triggered, this, &MainWindow::save);
    connect(m_ui->actionQuit,                       &QAction::triggered, this, &MainWindow::close);

    // Edit Menu
    connect(m_ui->actionCut,    &QAction::triggered, this, &MainWindow::cutTag);
    connect(m_ui->actionCopy,   &QAction::triggered, this, &MainWindow::copyTag);
    connect(m_ui->actionPaste,  &QAction::triggered, this, &MainWindow::pasteTag);
    connect(m_ui->actionRename, &QAction::triggered, this, &MainWindow::renameTag);
    connect(m_ui->actionEdit,   &QAction::triggered, this, &MainWindow::editTag);
    connect(m_ui->actionDelete, &QAction::triggered, this, &MainWindow::deleteTag);

    // Help Menu

    // Toolbar Add Tags
    connect(m_ui->actionAdd_ByteTag,        &QAction::triggered, this, &MainWindow::addByteTag);
    connect(m_ui->actionAdd_ShortTag,       &QAction::triggered, this, &MainWindow::addShortTag);
    connect(m_ui->actionAdd_IntTag,         &QAction::triggered, this, &MainWindow::addIntTag);
    connect(m_ui->actionAdd_LongTag,        &QAction::triggered, this, &MainWindow::addLongTag);
    connect(m_ui->actionAdd_FloatTag,       &QAction::triggered, this, &MainWindow::addFloatTag);
    connect(m_ui->actionAdd_DoubleTag,      &QAction::triggered, this, &MainWindow::addDoubleTag);
    connect(m_ui->actionAdd_StringTag,      &QAction::triggered, this, &MainWindow::addStringTag);
    connect(m_ui->actionAdd_ByteArrayTag,   &QAction::triggered, this, &MainWindow::addByteArrayTag);
    connect(m_ui->actionAdd_IntArrayTag,    &QAction::triggered, this, &MainWindow::addIntArrayTag);
    connect(m_ui->actionAdd_LongArrayTag,   &QAction::triggered, this, &MainWindow::addLongArrayTag);
    connect(m_ui->actionAdd_ListTag,        &QAction::triggered, this, &MainWindow::addListTag);
    connect(m_ui->actionAdd_CompoundTag,    &QAction::triggered, this, &MainWindow::addCompoundTag);
}

bool MainWindow::userReallyWantsToQuit()
{
    // Ask user here, if settings should be saved before quit.
    return true;
}

void MainWindow::addNbtTag(amc::TagType tagType)
{
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        NbtTreeItemBase *item = m_nbtTreeModel->fromIndex(index);
        if(dynamic_cast<NbtTreeItemListTag*>(item) != nullptr) {
            m_nbtTreeModel->addNbtTag(index, tagType, QString());
        } else {
            qDebug() << "Launch Dialog for new Tag!";
        }
    }
}

void MainWindow::updateActions()
{
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    NbtTreeItemBase *treeItem = m_nbtTreeModel->fromIndex(index);
    if(treeItem) {
        m_ui->actionRename->setEnabled(treeItem->canRename());

        m_ui->actionAdd_ByteTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::Byte));
        m_ui->actionAdd_ShortTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::Short));
        m_ui->actionAdd_IntTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::Int));
        m_ui->actionAdd_LongTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::Long));
        m_ui->actionAdd_FloatTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::Float));
        m_ui->actionAdd_DoubleTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::Double));
        m_ui->actionAdd_StringTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::String));
        m_ui->actionAdd_ByteArrayTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::ByteArray));
        m_ui->actionAdd_IntArrayTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::IntArray));
        m_ui->actionAdd_LongArrayTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::LongArray));
        m_ui->actionAdd_ListTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::List));
        m_ui->actionAdd_CompoundTag->setEnabled(treeItem->canAddNbtTag(amc::TagType::Compound));
    }
}

} // namespace anv