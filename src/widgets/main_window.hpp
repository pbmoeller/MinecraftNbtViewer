#ifndef AWESOMENBTVIEWER_WIDGETS_MAIN_WINDOW_HPP
#define AWESOMENBTVIEWER_WIDGETS_MAIN_WINDOW_HPP

// AwesomeMC
#include <AwesomeMC/nbt/tags/abstract_tag.hpp>

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
    void treeviewCurrentItemChanged(const QModelIndex &current, 
                                    const QModelIndex &previous);

    /// Slots to Actions
    // File Menu
    void openFile();
    void openFolder();
    void openMinecraftFolder();
    void openInExplorer();
    void save();

    // Edit Menu
    void cutTag();
    void copyTag();
    void pasteTag();
    void renameTag();
    void editTag();
    void deleteTag();

    // Add NBT Tag
    void addByteTag();
    void addShortTag();
    void addIntTag();
    void addLongTag();
    void addFloatTag();
    void addDoubleTag();
    void addStringTag();
    void addByteArrayTag();
    void addIntArrayTag();
    void addLongArrayTag();
    void addListTag();
    void addCompoundTag();

private:
    void initConnections();
    bool userReallyWantsToQuit();
    void addNbtTag(amc::TagType tagType);
    void updateActions();

private:
    Ui::MainWindow *m_ui;

    QString m_currentDirectory;

    NbtDataTreeModel *m_nbtTreeModel;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_WIDGETS_MAIN_WINDOW_HPP