// AwesomeNbtViewer
#include "util/minecraft_util.hpp"

// AwesomeMC
#include <cpp-anvil/nbt/io.hpp>

// Qt
#include <QFileInfo>

namespace anv
{

bool isMinecraftWorldDirectory(const QString &directory)
{
    // Add more files to check if there are false results
    QFileInfo levelDatFile(directory + "/level.dat");
    return levelDatFile.exists()
        && anvil::isNbtFile(levelDatFile.absoluteFilePath().toStdString());
}

} // namespace anv