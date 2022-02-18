#include "main_window.hpp"
#include "ui_main_window.h"
#include "models/nbt_data_treemodel.hpp"

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

    m_nbtTreeModel = new NbtDataTreeModel();
    m_ui->nbtDataTreeView->setModel(m_nbtTreeModel);

    m_currentDirectory = QStandardPaths::AppDataLocation + "\\.minecraft\\saves";

    connect(m_ui->actionOpen_File, &QAction::triggered, this, &MainWindow::openFile);
    connect(m_ui->actionOpen_Folder, &QAction::triggered, this, &MainWindow::openFolder);
    connect(m_ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
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

        NbtDataTreeModel *model = new NbtDataTreeModel(filename, this);
        m_ui->nbtDataTreeView->setModel(model);
    }
}

void MainWindow::openFolder()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Folder"), m_currentDirectory);
    if(!directory.isEmpty()) {
        qDebug() << "Opening directory: " << directory;


    }
}

bool MainWindow::userReallyWantsToQuit()
{
    // Ask user here, if settings should be saved before quit.
    return true;
}

} // namespace anv