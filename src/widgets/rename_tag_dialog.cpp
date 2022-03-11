#include "rename_tag_dialog.hpp"
#include "ui_rename_tag_dialog.h"

namespace anv
{

RenameTagDialog::RenameTagDialog(QString currentName, QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::RenameTagDialog)
{
    m_ui->setupUi(this);
    m_ui->nameEdit->setText(currentName);
    m_ui->nameEdit->selectAll();
}
RenameTagDialog::~RenameTagDialog()
{
    delete m_ui;
}

QString RenameTagDialog::getName() const
{
    return m_ui->nameEdit->text();
}

} // namespace anv