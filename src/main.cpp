// AwesomeNbtViewer
#include "core/awesome_nbt_viewer_app.hpp"
#include "util/tag_mime_data.hpp"
#include "version.hpp"

// AwesomeMC
#include <cpp-anvil/version.hpp>

// Qt
#include <QApplication>

// STL
#include <iostream>

int main(int argc, char **argv)
{
    std::cout << "AwesomeMC Version: " << anvil::getCppAnvilVersion() << std::endl;
    std::cout << "AwesomeNbtViewer Version: " << anv::getVersion() << std::endl;
    qRegisterMetaType<std::shared_ptr<anvil::BasicTag>>();

    QCoreApplication::setApplicationName("Awesome Nbt Viewer");
    QCoreApplication::setApplicationVersion(anv::getVersion());

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/grass_block_64x64.png"));
    anv::AwesomeNbtViewerApp awesomeApp;
    awesomeApp.create();
    return app.exec();
}