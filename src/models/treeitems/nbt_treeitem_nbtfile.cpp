// AwesomeNbtViewer
#include "nbt_treeitem_nbtfile.hpp"

namespace anv
{

NbtTreeItemNbtFile::NbtTreeItemNbtFile(NbtTreeItemBase *parentItem,
                                       const QString &filename,
                                       const QString &pathToFile)
    : NbtTreeItemBase(parentItem)
    , m_canFetchData(true)
    , m_filename(filename)
    , m_pathToFile(pathToFile)
{

}

NbtTreeItemNbtFile::~NbtTreeItemNbtFile()
{

}

QIcon NbtTreeItemNbtFile::getIcon() const
{
    return QIcon(":/icons/16x16/NbtFile.png");
}

QString NbtTreeItemNbtFile::getName() const
{
    return m_filename;
}

bool NbtTreeItemNbtFile::canFetchMore() const
{
    return m_canFetchData;
}

void NbtTreeItemNbtFile::fetchMore()
{
    m_canFetchData = false;
}

} // namespace anv