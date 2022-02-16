#include <AwesomeNbtViewer/widgets/main_window.hpp>
#include <AwesomeNbtViewer/widgets/ui_main_window.h>

namespace anv
{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

} // namespace anv