#ifndef MINECRAFTNBTVIEWER_VERSION_HPP_IN
#define MINECRAFTNBTVIEWER_VERSION_HPP_IN

namespace minecraft {
namespace nbt {

unsigned int getVersionMajor();
unsigned int getVersionMinor();
unsigned int getVersionPatch();
unsigned int getVersionTweak();

const char * getVersion();
const char * getGitBranch();
const char * getGitHash();

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_VERSION_HPP_IN
