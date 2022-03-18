#ifndef AWESOMENBTVIEWER_WIDGETS_EDIT_VALUE_DIALOG_HPP
#define AWESOMENBTVIEWER_WIDGETS_EDIT_VALUE_DIALOG_HPP

// AwesomeNbtViewer
#include "models/nbt_data_treemodel.hpp"
#include "models/treeitems/nbt_treeitem_nbttag.hpp"

// Qt
#include <QDialog>
#include <QString>

namespace Ui
{
class EditValueDialog;
}

namespace anv
{

class EditValueDialog : public QDialog
{
    Q_OBJECT

public:
    EditValueDialog(NbtTreeItemNbtTag *treeItem, NbtDataTreeModel *model, QWidget *parent = nullptr);
    ~EditValueDialog();

private:
    QString valueToString();

protected slots:
    virtual void accept() override;

private:
    Ui::EditValueDialog *m_ui;
    NbtTreeItemNbtTag *m_treeItem;
    NbtDataTreeModel *m_model;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_WIDGETS_EDIT_VALUE_DIALOG_HPP