// MinecraftNbtViewer
#include "core/minecraft_nbt_viewer_app.hpp"
#include "util/tag_mime_data.hpp"
#include "version.hpp"

// cpp-anvil
#include <cpp-anvil/version.hpp>

// Qt
#include <QApplication>
#include <QDebug>

// STL
#include <iostream>

int main(int argc, char** argv)
{
    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz}] ["
                       "%{if-debug}Debug%{endif}%{if-info}Info%{endif}%{if-warning}Warning%{endif}"
                       "%{if-critical}Critical%{endif}%{if-fatal}Fatal%{endif}] : %{message}");

    qInfo() << "cpp-anvil Version: " << anvil::getCppAnvilVersion();
    qInfo() << "MinecraftNbtViewer Version: " << minecraft::nbt::getVersion();
    qRegisterMetaType<std::shared_ptr<anvil::BasicTag>>();

    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(minecraft::nbt::getApplicationName());
    QCoreApplication::setApplicationVersion(minecraft::nbt::getVersion());

    app.setWindowIcon(QIcon(":/icons/grass_block_64x64.png"));
    minecraft::nbt::MinecraftNbtViewerApp mApp;
    mApp.create();
    return app.exec();
}
