#include "main_window.hpp"
#include "ui_main_window.h"
#include "models/nbt_data_treemodel.hpp"
#include "models/treeitems/nbt_treeitem_nbttag.hpp"

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
    /// Menu
    // File Menu
    connect(m_ui->actionOpen_File,                  &QAction::triggered, this, &MainWindow::openFile);
    connect(m_ui->actionOpen_Folder,                &QAction::triggered, this, &MainWindow::openFolder);
    connect(m_ui->actionOpen_Minecraft_Save_Folder, &QAction::triggered, this, &MainWindow::openMinecraftFolder);
    connect(m_ui->actionQuit,                       &QAction::triggered, this, &MainWindow::close);

    // Edit Menu

    // Help Menu

    /// Toolbar
    // File

    // Edit

    // Add Tags
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

} // namespace anv