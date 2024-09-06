#ifndef MINECRAFTNBTVIEWER_WIDGETS_NEW_TAG_DIALOG_HPP
#define MINECRAFTNBTVIEWER_WIDGETS_NEW_TAG_DIALOG_HPP

// MinecraftNbtViewer
#include "models/treeitems/nbt_treeitem_nbttag.hpp"

// Qt
#include <QDialog>
#include <QString>

namespace Ui {
class NewTagDialog;
}

namespace minecraft {
namespace nbt {

class NewTagDialog : public QDialog
{
    Q_OBJECT

public:
    NewTagDialog(NbtTreeItemNbtTag *parentItem,
                 anvil::TagType newItemTagType,
                 QWidget *parent = nullptr);
    ~NewTagDialog();

    QString name() const;
    int size() const;

protected slots:
    virtual void accept() override;

private:
    Ui::NewTagDialog *m_ui;
    NbtTreeItemNbtTag *m_parentItem;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_WIDGETS_NEW_TAG_DIALOG_HPP
