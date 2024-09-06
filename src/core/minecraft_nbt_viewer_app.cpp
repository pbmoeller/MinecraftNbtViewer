// MinecraftNbtViewer
#include "minecraft_nbt_viewer_app.hpp"

namespace minecraft {
namespace nbt {

MinecraftNbtViewerApp::MinecraftNbtViewerApp(QObject *parent)
    : QObject(parent)
    , m_mainWindow()
{

}

void MinecraftNbtViewerApp::create()
{
    m_mainWindow.show();
}

} // namespace nbt
} // namespace minecraft
