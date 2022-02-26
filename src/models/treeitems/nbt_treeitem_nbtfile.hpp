#ifndef AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTFILE_HPP
#define AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTFILE_HPP

// AwesomeNbtViewer
#include "nbt_treeitem_base.hpp"

// Qt
#include <QString>
#include <QIcon>

namespace anv
{

class NbtTreeItemNbtFile : public NbtTreeItemBase
{
public:
    NbtTreeItemNbtFile(NbtTreeItemBase *parentItem,
                       const QString &filename,
                       const QString &pathToFile);
    ~NbtTreeItemNbtFile();

    virtual QIcon getIcon() const override;
    virtual QString getName() const override;

    virtual bool canFetchMore() const override;
    virtual void fetchMore() override;

private:
    bool m_canFetchData;
    QString m_filename;
    QString m_pathToFile;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_MODELS_TREEITEMS_NBT_TREEITEM_NBTFILE_HPP