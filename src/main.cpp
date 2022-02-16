// AwesomeNbtViewer
#include "core/awesome_nbt_viewer_app.hpp"
#include "version.hpp"

// Qt
#include <QApplication>

// STL
#include <iostream>

int main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;

    QCoreApplication::setApplicationName("AwesomeNbtViewer");
    QCoreApplication::setApplicationVersion(anv::getVersion());

    QApplication app(argc, argv);
    anv::AwesomeNbtViewerApp awesomeApp;
    awesomeApp.create();
    return app.exec();
}