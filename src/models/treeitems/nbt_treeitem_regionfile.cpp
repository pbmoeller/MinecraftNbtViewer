//AwesomeNbtViewer
#include "nbt_treeitem_regionfile.hpp"

namespace anv
{

NbtTreeItemRegionFile::NbtTreeItemRegionFile(NbtTreeItemBase *parentItem,
                                             const QString &filename,
                                             const QString &pathToFile)
    : NbtTreeItemBase(parentItem)
    , m_canFetchData(true)
    , m_filename(filename)
    , m_pathToFile(pathToFile)
{

}

NbtTreeItemRegionFile::~NbtTreeItemRegionFile()
{

}

QIcon NbtTreeItemRegionFile::getIcon() const
{
    return QIcon(":/icons/16x16/Region.png");
}

QString NbtTreeItemRegionFile::getName() const
{
    return m_filename;
}

bool NbtTreeItemRegionFile::canFetchMore() const
{
    return m_canFetchData;
}

void NbtTreeItemRegionFile::fetchMore()
{
    m_canFetchData = false;
}

} // namespace anv