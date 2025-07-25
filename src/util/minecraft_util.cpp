// MinecraftNbtViewer
#include "util/minecraft_util.hpp"

// cpp-anvil
#include <cpp-anvil/nbt/io.hpp>

// Qt
#include <QFileInfo>

namespace minecraft {
namespace nbt {

bool isMinecraftWorldDirectory(const QString& directory)
{
    // Add more files to check if there are false results
    QFileInfo levelDatFile(directory + "/level.dat");
    return levelDatFile.exists() && anvil::isNbtFile(levelDatFile.absoluteFilePath().toStdString());
}

} // namespace nbt
} // namespace minecraft
