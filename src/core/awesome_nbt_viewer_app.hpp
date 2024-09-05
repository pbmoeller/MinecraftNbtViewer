#ifndef MINECRAFTNBTVIEWER_CORE_MINECRAFT_NBT_VIEWER_APP_HPP
#define MINECRAFTNBTVIEWER_CORE_MINECRAFT_NBT_VIEWER_APP_HPP

// MinecraftNbtViewer
#include "widgets/main_window.hpp"

// Qt
#include <QObject>

namespace anv
{

class MinecraftNbtViewerApp : public QObject
{
public:
    MinecraftNbtViewerApp(QObject *parent = nullptr);

    void create();

private:
    MainWindow m_mainWindow;
};

} // namespace anv

#endif // MINECRAFTNBTVIEWER_CORE_MINECRAFT_NBT_VIEWER_APP_HPP
