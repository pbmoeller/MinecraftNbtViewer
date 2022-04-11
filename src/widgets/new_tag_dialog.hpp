#ifndef AWESOMENBTVIEWER_WIDGETS_NEW_TAG_DIALOG_HPP
#define AWESOMENBTVIEWER_WIDGETS_NEW_TAG_DIALOG_HPP

// AwesomeNbtViewer
#include "models/treeitems/nbt_treeitem_nbttag.hpp"

// Qt
#include <QDialog>
#include <QString>

namespace Ui
{
class NewTagDialog;
}

namespace anv
{

class NewTagDialog : public QDialog
{
    Q_OBJECT

public:
    NewTagDialog(NbtTreeItemNbtTag *parentItem,
                 amc::TagType newItemTagType,
                 QWidget *parent = nullptr);
    ~NewTagDialog();

    QString getName() const;
    int getSize() const;

protected slots:
    virtual void accept() override;

private:
    Ui::NewTagDialog *m_ui;
    NbtTreeItemNbtTag *m_parentItem;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_WIDGETS_NEW_TAG_DIALOG_HPP