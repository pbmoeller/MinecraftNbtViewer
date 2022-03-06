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

class NbtDataTreeModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void closeEvent(QCloseEvent *event);

    void openFile();
    void openFolder();
    void openMinecraftFolder();

private:
    bool userReallyWantsToQuit();

private:
    Ui::MainWindow *m_ui;

    QString m_currentDirectory;

    NbtDataTreeModel *m_nbtTreeModel;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_WIDGETS_MAIN_WINDOW_HPP