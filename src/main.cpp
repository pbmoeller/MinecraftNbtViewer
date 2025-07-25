// MinecraftNbtViewer
#include "core/minecraft_nbt_viewer_app.hpp"
#include "util/tag_mime_data.hpp"
#include "version.hpp"

// cpp-anvil
#include <cpp-anvil/version.hpp>

// Qt
#include <QApplication>

// STL
#include <iostream>

int main(int argc, char **argv)
{
    std::cout << "cpp-anvil Version: " << anvil::getCppAnvilVersion() << std::endl;
    std::cout << "MinecraftNbtViewer Version: " << minecraft::nbt::getVersion() << std::endl;
    qRegisterMetaType<std::shared_ptr<anvil::BasicTag>>();

    QCoreApplication::setApplicationName("MinecraftNbtViewer");
    QCoreApplication::setApplicationVersion(minecraft::nbt::getVersion());

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/grass_block_64x64.png"));
    minecraft::nbt::MinecraftNbtViewerApp mApp;
    mApp.create();
    return app.exec();
}