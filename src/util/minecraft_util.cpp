// AwesomeNbtViewer
#include "util/minecraft_util.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/nbt_read.hpp>

// Qt
#include <QFileInfo>

namespace anv
{

bool isMinecraftWorldDirectory(const QString &directory)
{
    // Add more files to check if there are false results
    QFileInfo levelDatFile(directory + "/level.dat");
    return levelDatFile.exists()
        && amc::isNbtFile(levelDatFile.absoluteFilePath().toStdString());
}

} // namespace anv