#ifndef MINECRAFTNBTVIEWER_CORE_MINECRAFT_NBT_VIEWER_APP_HPP
#define MINECRAFTNBTVIEWER_CORE_MINECRAFT_NBT_VIEWER_APP_HPP

// MinecraftNbtViewer
#include "widgets/main_window.hpp"

// Qt
#include <QObject>

namespace minecraft {
namespace nbt {

class MinecraftNbtViewerApp : public QObject
{
public:
    MinecraftNbtViewerApp(QObject* parent = nullptr);

    void create();

private:
    MainWindow m_mainWindow;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_CORE_MINECRAFT_NBT_VIEWER_APP_HPP
