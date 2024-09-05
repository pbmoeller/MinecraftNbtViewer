#ifndef MINECRAFTNBTVIEWER_VERSION_HPP_IN
#define MINECRAFTNBTVIEWER_VERSION_HPP_IN

namespace anv {

int getVersionMajor();
int getVersionMinor();
int getVersionPatch();

const char * getVersion();
const char * getGitBranch();
const char * getGitHash();

} // namespace anv

#endif // MINECRAFTNBTVIEWER_VERSION_HPP_IN
