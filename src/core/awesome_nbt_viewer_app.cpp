#include "awesome_nbt_viewer_app.hpp"

namespace anv
{

AwesomeNbtViewerApp::AwesomeNbtViewerApp(QObject *parent)
    : QObject(parent)
    , m_mainWindow()
{

}

void AwesomeNbtViewerApp::create()
{
    m_mainWindow.show();
}

} // namespace anv