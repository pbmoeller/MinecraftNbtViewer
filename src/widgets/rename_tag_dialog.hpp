#ifndef AWESOMENBTVIEWER_WIDGETS_RENAME_TAG_DIALOG_HPP
#define AWESOMENBTVIEWER_WIDGETS_RENAME_TAG_DIALOG_HPP

// Qt
#include <QDialog>
#include <QString>

namespace Ui
{
class RenameTagDialog;
}

namespace anv
{

class RenameTagDialog : public QDialog
{
    Q_OBJECT

public:
    RenameTagDialog(QString currentName, QWidget *parent = nullptr);
    ~RenameTagDialog();

public:
    QString getName() const;

private:
    Ui::RenameTagDialog *m_ui;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_WIDGETS_RENAME_TAG_DIALOG_HPP