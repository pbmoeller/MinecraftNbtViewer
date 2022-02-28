// AwesomeNbtViewer
#include "nbt_treeitem_nbtfile.hpp"
#include "nbt_treeitem_nbttag.hpp"
#include "treeitem_util.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/nbt_read.hpp>

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

    // Read NBT data
    std::string filename = (m_pathToFile + "/" + m_filename).toStdString();
    m_nbtRootTag = amc::readNbtFile(filename);
    if(m_nbtRootTag) {
        amc::CompoundTag *tag = amc::tag_cast<amc::CompoundTag*>(m_nbtRootTag.get());
        for(amc::AbstractTag *childTag : tag->getValue()) {
            addNbtChild(this, childTag);
        }

        sort();
    }
}

} // namespace anv