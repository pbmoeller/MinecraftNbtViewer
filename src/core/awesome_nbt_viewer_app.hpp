#ifndef AWESOMENBTVIEWER_CORE_AWESOME_NBT_VIEWER_APP_HPP
#define AWESOMENBTVIEWER_CORE_AWESOME_NBT_VIEWER_APP_HPP

// AwesomeNbtViewer
#include "widgets/main_window.hpp"

// Qt
#include <QObject>

namespace anv
{

class AwesomeNbtViewerApp : public QObject
{
public:
    AwesomeNbtViewerApp(QObject *parent = nullptr);

    void create();

private:
    MainWindow m_mainWindow;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_CORE_AWESOME_NBT_VIEWER_APP_HPP