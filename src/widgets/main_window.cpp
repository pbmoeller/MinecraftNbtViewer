#include "main_window.hpp"
#include "ui_main_window.h"
#include "models/nbt_data_treemodel.hpp"
#include "models/treeitems/nbt_treeitem_nbttag.hpp"
#include "models/treeitems/nbt_treeitem_nbtfile.hpp"
#include "models/treeitems/nbt_treeitem_folder.hpp"
#include "new_tag_dialog.hpp"
#include "compression_dialog.hpp"

// Qt
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>

namespace anv
{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_baseWindowTitle(QApplication::applicationName())
{
    m_ui->setupUi(this);
    this->setWindowTitle(m_baseWindowTitle);

    m_nbtTreeModel = new NbtDataTreeModel();
    m_ui->nbtDataTreeView->setModel(m_nbtTreeModel);
    m_ui->nbtDataTreeView->setHeaderHidden(true);
    m_ui->nbtDataTreeView->setIconSize(QSize(16, 16));
    m_ui->nbtDataTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    initConnections();

    // Load default Minecraft directory
    openMinecraftFolder();

    updateActions();
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

void MainWindow::modifiedModel()
{
    QString windowTitle = m_baseWindowTitle;
    if(m_nbtTreeModel->isModified()) {
        windowTitle += " [*]";
    }
    setWindowTitle(windowTitle);
    setWindowModified(m_nbtTreeModel->isModified());

    updateActions();
}

void MainWindow::newNbtFile()
{
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        m_nbtTreeModel->addNewNbtFile(index);
    }
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
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        NbtTreeItemBase *treeItem = m_nbtTreeModel->fromIndex(index);
        QString path = treeItem->getPath();
        QDesktopServices::openUrl(QUrl("file:///" + path, QUrl::TolerantMode));
    }
}

void MainWindow::save()
{
    qDebug() << "Save";
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        m_nbtTreeModel->save(index);
    }
}

void MainWindow::saveAs()
{
    qDebug() << "Save As ...";

    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        NbtTreeItemNbtFile *treeItem = dynamic_cast<NbtTreeItemNbtFile*>(m_nbtTreeModel->fromIndex(index));
        if(!treeItem) {
            return;
        }

        QString currentFilename = treeItem->getName();
        QString path = treeItem->getPath();
        QString filename = QFileDialog::getSaveFileName(this, tr("Save %1 as...").arg(currentFilename), path);
        
        if(!filename.isNull()) {
            CompressionDialog compressionDiag(treeItem->getCompression());
            compressionDiag.exec();
            if(compressionDiag.result() == QDialog::Accepted) {
                m_nbtTreeModel->saveAs(index, filename, compressionDiag.getSelectedCompressionType());
            }
        }
    }
}

void MainWindow::saveAll()
{
    qDebug() << "Save All";
    m_nbtTreeModel->saveAll();
}

void MainWindow::refresh()
{
    qDebug() << "Refresh";
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        if(m_nbtTreeModel->isDirty(index)) {
            QMessageBox::StandardButton btn;
            btn = QMessageBox::warning(this,
                                       tr("%1 - Unsaved Changes").arg(m_baseWindowTitle),
                                       tr("The file has unsaved changes.\n\n"
                                          "Are you sure to continue without saving?\n"
                                          "Your changes will be discarded."),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);
            if(btn == QMessageBox::No) {
                return;
            }
        }

        m_nbtTreeModel->refresh(index);
    }
}

void MainWindow::cutTag()
{
    qDebug() << "Cut";
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        m_nbtTreeModel->cutTag(index);
    }
}

void MainWindow::copyTag()
{
    qDebug() << "Copy";
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        m_nbtTreeModel->copyTag(index);
    }
}

void MainWindow::pasteTag()
{
    qDebug() << "Paste";
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        m_nbtTreeModel->pasteTag(index);
    }
}

void MainWindow::renameTag()
{
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        m_nbtTreeModel->renameTag(index);
    }
}

void MainWindow::editTag()
{
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        m_nbtTreeModel->editTag(index);
    }
}

void MainWindow::deleteTag()
{
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        m_nbtTreeModel->deleteTag(index);
    }
}

void MainWindow::moveUp()
{
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        m_nbtTreeModel->moveUp(index);
    }
}

void MainWindow::moveDown()
{
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        m_nbtTreeModel->moveDown(index);
    }
}

void MainWindow::addByteTag()
{
    addNbtTag(anvil::TagType::Byte);
}

void MainWindow::addShortTag()
{
    addNbtTag(anvil::TagType::Short);
}

void MainWindow::addIntTag()
{
    addNbtTag(anvil::TagType::Int);
}

void MainWindow::addLongTag()
{
    addNbtTag(anvil::TagType::Long);
}

void MainWindow::addFloatTag()
{
    addNbtTag(anvil::TagType::Float);
}

void MainWindow::addDoubleTag()
{
    addNbtTag(anvil::TagType::Double);
}

void MainWindow::addStringTag()
{
    addNbtTag(anvil::TagType::String);
}

void MainWindow::addByteArrayTag()
{
    addNbtTag(anvil::TagType::ByteArray);
}

void MainWindow::addIntArrayTag()
{
    addNbtTag(anvil::TagType::IntArray);
}

void MainWindow::addLongArrayTag()
{
    addNbtTag(anvil::TagType::LongArray);
}

void MainWindow::addListTag()
{
    addNbtTag(anvil::TagType::List);
}

void MainWindow::addCompoundTag()
{
    addNbtTag(anvil::TagType::Compound);
}

void MainWindow::initConnections()
{
    /// General UI
    connect(m_ui->nbtDataTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::treeviewCurrentItemChanged);
    connect(m_ui->nbtDataTreeView, &QTreeView::customContextMenuRequested,
            this, &MainWindow::showCustomContextMenu);
    connect(m_ui->nbtDataTreeView, &QTreeView::doubleClicked,
            this, &MainWindow::editTag);
    connect(this->m_nbtTreeModel, &NbtDataTreeModel::modified,
            this, &MainWindow::modifiedModel);

    /// Actions
    // File Menu
    connect(m_ui->actionNew_NBT_File,               &QAction::triggered, this, &MainWindow::newNbtFile);
    connect(m_ui->actionOpen_File,                  &QAction::triggered, this, &MainWindow::openFile);
    connect(m_ui->actionOpen_Folder,                &QAction::triggered, this, &MainWindow::openFolder);
    connect(m_ui->actionOpen_Minecraft_Save_Folder, &QAction::triggered, this, &MainWindow::openMinecraftFolder);
    connect(m_ui->actionOpen_in_Explorer,           &QAction::triggered, this, &MainWindow::openInExplorer);
    connect(m_ui->actionSave,                       &QAction::triggered, this, &MainWindow::save);
    connect(m_ui->actionSaveAs,                     &QAction::triggered, this, &MainWindow::saveAs);
    connect(m_ui->actionSaveAll,                    &QAction::triggered, this, &MainWindow::saveAll);
    connect(m_ui->actionRefresh,                    &QAction::triggered, this, &MainWindow::refresh);
    connect(m_ui->actionQuit,                       &QAction::triggered, this, &MainWindow::close);

    // Edit Menu
    connect(m_ui->actionCut,        &QAction::triggered, this, &MainWindow::cutTag);
    connect(m_ui->actionCopy,       &QAction::triggered, this, &MainWindow::copyTag);
    connect(m_ui->actionPaste,      &QAction::triggered, this, &MainWindow::pasteTag);
    connect(m_ui->actionRename,     &QAction::triggered, this, &MainWindow::renameTag);
    connect(m_ui->actionEdit,       &QAction::triggered, this, &MainWindow::editTag);
    connect(m_ui->actionDelete,     &QAction::triggered, this, &MainWindow::deleteTag);
    connect(m_ui->actionMoveUp,     &QAction::triggered, this, &MainWindow::moveUp);
    connect(m_ui->actionMoveDown,   &QAction::triggered, this, &MainWindow::moveDown);

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
    if(m_nbtTreeModel->isModified()) {
        QMessageBox::StandardButton btn;
        btn = QMessageBox::warning(this, m_baseWindowTitle, tr("The application has unsaved changes.\n"
                                                               "Do you want to save your changes?"),
                                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                   QMessageBox::Yes);
        if(btn == QMessageBox::Yes || btn == QMessageBox::Cancel) {
            return false;
        }
    }

    return true;
}

void MainWindow::addNbtTag(anvil::TagType tagType)
{
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    if(index.isValid()) {
        NbtTreeItemBase *item = m_nbtTreeModel->fromIndex(index);
        NbtTreeItemNbtTag *tagItem = dynamic_cast<NbtTreeItemNbtTag*>(item);
        if(!tagItem) {
            return;
        }

        if(tagItem->getTagType() == anvil::TagType::List) {
            m_nbtTreeModel->addNbtTag(index, tagItem, tagType, QString());
        } else {
            qDebug() << "Launch Dialog for new Tag!";
            NewTagDialog newTagDialog(tagItem, tagType);
            if(newTagDialog.exec() == QDialog::Accepted) {
                QString tagName = newTagDialog.getName();
                int size = newTagDialog.getSize();
                m_nbtTreeModel->addNbtTag(index, tagItem, tagType, tagName, size);
            }
        }
    }
}

void MainWindow::updateActions()
{
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    NbtTreeItemBase *treeItem = m_nbtTreeModel->fromIndex(index);
    if(treeItem) {
        m_ui->actionNew_NBT_File->setEnabled(treeItem->canAddNbtFile());
        m_ui->actionOpen_in_Explorer->setEnabled(treeItem->canOpenInExplorer());
        m_ui->actionSave->setEnabled(treeItem->canSave());
        m_ui->actionRefresh->setEnabled(treeItem->canRefresh());

        m_ui->actionRename->setEnabled(treeItem->canRename());
        m_ui->actionEdit->setEnabled(treeItem->canEdit());
        m_ui->actionDelete->setEnabled(treeItem->canDelete());

        m_ui->actionCut->setEnabled(treeItem->canCut());
        m_ui->actionCopy->setEnabled(treeItem->canCopy());
        m_ui->actionPaste->setEnabled(treeItem->canPaste());

        m_ui->actionMoveUp->setEnabled(treeItem->canMoveUp());
        m_ui->actionMoveDown->setEnabled(treeItem->canMoveDown());

        m_ui->actionAdd_ByteTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::Byte));
        m_ui->actionAdd_ShortTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::Short));
        m_ui->actionAdd_IntTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::Int));
        m_ui->actionAdd_LongTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::Long));
        m_ui->actionAdd_FloatTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::Float));
        m_ui->actionAdd_DoubleTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::Double));
        m_ui->actionAdd_StringTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::String));
        m_ui->actionAdd_ByteArrayTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::ByteArray));
        m_ui->actionAdd_IntArrayTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::IntArray));
        m_ui->actionAdd_LongArrayTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::LongArray));
        m_ui->actionAdd_ListTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::List));
        m_ui->actionAdd_CompoundTag->setEnabled(treeItem->canAddNbtTag(anvil::TagType::Compound));
    }
}

void MainWindow::showCustomContextMenu(const QPoint &pos)
{
    Q_UNUSED(pos);

    QMenu *contextMenu = new QMenu(this);
    QModelIndex index = m_ui->nbtDataTreeView->currentIndex();
    NbtTreeItemBase *treeItem = m_nbtTreeModel->fromIndex(index);
    
    if(!treeItem) {
        return;
    }

    // Folder
    if(dynamic_cast<NbtTreeItemFolder*>(treeItem)) {
        contextMenu->addAction(m_ui->actionOpen_in_Explorer);
    }
    if(treeItem->canRefresh()) {
        contextMenu->addAction(m_ui->actionRefresh);
    }

    // Rename - Edit - Delete
    QList<QAction*> renameEditDeleteActions;
    if(treeItem->canRename()) {
        renameEditDeleteActions.append(m_ui->actionRename);
    }
    if(treeItem->canEdit()) {
        renameEditDeleteActions.append(m_ui->actionEdit);
    }
    if(treeItem->canDelete()) {
        renameEditDeleteActions.append(m_ui->actionDelete);
    }
    if(!renameEditDeleteActions.isEmpty()) {
        contextMenu->addSeparator();
        contextMenu->addActions(renameEditDeleteActions);
    }

    // Cut - Copy - Paste
    QList<QAction*> cutCopyPasteActions;
    if(treeItem->canCut()) {
        cutCopyPasteActions.append(m_ui->actionCut);
    }
    if(treeItem->canCopy()) {
        cutCopyPasteActions.append(m_ui->actionCopy);
    }
    if(treeItem->canPaste()) {
        cutCopyPasteActions.append(m_ui->actionPaste);
    }
    if(!cutCopyPasteActions.isEmpty()) {
        contextMenu->addSeparator();
        contextMenu->addActions(cutCopyPasteActions);
    }

    // Moving
    QList<QAction*> moveActions;
    if(treeItem->canMoveUp()) {
        moveActions.append(m_ui->actionMoveUp);
    }
    if(treeItem->canMoveDown()) {
        moveActions.append(m_ui->actionMoveDown);
    }
    if(!moveActions.isEmpty()) {
        contextMenu->addSeparator();
        contextMenu->addActions(moveActions);
    }

    // Add Tags
    QList<QAction*> addTagActions;
    if(treeItem->canAddNbtTag(anvil::TagType::Byte)) {
        addTagActions.append(m_ui->actionAdd_ByteTag);
    }
    if(treeItem->canAddNbtTag(anvil::TagType::Short)) {
        addTagActions.append(m_ui->actionAdd_ShortTag);
    }
    if(treeItem->canAddNbtTag(anvil::TagType::Int)) {
        addTagActions.append(m_ui->actionAdd_IntTag);
    }
    if(treeItem->canAddNbtTag(anvil::TagType::Long)) {
        addTagActions.append(m_ui->actionAdd_LongTag);
    }
    if(treeItem->canAddNbtTag(anvil::TagType::Float)) {
        addTagActions.append(m_ui->actionAdd_FloatTag);
    }
    if(treeItem->canAddNbtTag(anvil::TagType::Double)) {
        addTagActions.append(m_ui->actionAdd_DoubleTag);
    }
    if(treeItem->canAddNbtTag(anvil::TagType::String)) {
        addTagActions.append(m_ui->actionAdd_StringTag);
    }
    if(treeItem->canAddNbtTag(anvil::TagType::ByteArray)) {
        addTagActions.append(m_ui->actionAdd_ByteArrayTag);
    }
    if(treeItem->canAddNbtTag(anvil::TagType::IntArray)) {
        addTagActions.append(m_ui->actionAdd_IntArrayTag);
    }
    if(treeItem->canAddNbtTag(anvil::TagType::LongArray)) {
        addTagActions.append(m_ui->actionAdd_LongArrayTag);
    }
    if(treeItem->canAddNbtTag(anvil::TagType::List)) {
        addTagActions.append(m_ui->actionAdd_ListTag);
    }
    if(treeItem->canAddNbtTag(anvil::TagType::Compound)) {
        addTagActions.append(m_ui->actionAdd_CompoundTag);
    }
    if(!addTagActions.isEmpty()) {
        contextMenu->addSeparator();
        contextMenu->addActions(addTagActions);
    }

    // Show Menu
    contextMenu->popup(m_ui->nbtDataTreeView->viewport()->mapToGlobal(pos));
}

} // namespace anv