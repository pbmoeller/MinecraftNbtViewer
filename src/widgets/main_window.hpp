#ifndef MINECRAFTNBTVIEWER_WIDGETS_MAIN_WINDOW_HPP
#define MINECRAFTNBTVIEWER_WIDGETS_MAIN_WINDOW_HPP

// cpp-anvil
#include <cpp-anvil/nbt/types.hpp>

// Qt
#include <QMainWindow>

namespace Ui {
class MainWindow;
} // namespace Ui

namespace minecraft {
namespace nbt {

class NbtDataTreeModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void closeEvent(QCloseEvent* event);
    void treeviewCurrentItemChanged(const QModelIndex& current, const QModelIndex& previous);
    void modifiedModel();

    /// Slots to Actions
    // File Menu
    void newNbtFile();
    void openFile();
    void openFolder();
    void openMinecraftFolder();
    void openInExplorer();
    void save();
    void saveAs();
    void saveAll();
    void refresh();

    // Edit Menu
    void cutTag();
    void copyTag();
    void pasteTag();
    void renameTag();
    void editTag();
    void deleteTag();
    void moveUp();
    void moveDown();

    // Help Menu
    void about();

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
    void addNbtTag(anvil::TagType tagType);
    void updateActions();
    bool isDarkMode() const;
    void updateActionIcons();

private slots:
    void showCustomContextMenu(const QPoint& pos);

private:
    Ui::MainWindow* m_ui;

    QString m_currentDirectory;
    QString m_baseWindowTitle;

    NbtDataTreeModel* m_nbtTreeModel;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_WIDGETS_MAIN_WINDOW_HPP
