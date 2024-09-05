// MinecraftNbtViewer
#include "minecraft_nbt_viewer_app.hpp"

namespace anv
{

MinecraftNbtViewerApp::MinecraftNbtViewerApp(QObject *parent)
    : QObject(parent)
    , m_mainWindow()
{

}

void MinecraftNbtViewerApp::create()
{
    m_mainWindow.show();
}

} // namespace anv
