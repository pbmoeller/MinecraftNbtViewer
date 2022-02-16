#ifndef AWESOMENBTVIEWER_WIDGETS_MAIN_WINDOW_HPP
#define AWESOMENBTVIEWER_WIDGETS_MAIN_WINDOW_HPP

// Qt
#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

namespace anv
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *m_ui;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_WIDGETS_MAIN_WINDOW_HPP