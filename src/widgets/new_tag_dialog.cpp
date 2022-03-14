// AwesomeNbtViewer
#include "new_tag_dialog.hpp"
#include "ui_new_tag_dialog.h"

// Qt
#include <QMessageBox>
#include <QIntValidator>

// STL
#include <climits>

namespace anv
{

QString tagNames[14] = {
    "End",
    "Byte",
    "Short",
    "Int",
    "Long",
    "Float",
    "Double",
    "ByteArray",
    "String",
    "List",
    "Compound",
    "IntArray",
    "LongArray"
};

NewTagDialog::NewTagDialog(NbtTreeItemNbtTag *parentItem, QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::NewTagDialog)
    , m_parentItem(parentItem)
{
    m_ui->setupUi(this);
    m_ui->sizeEdit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), this));
    m_ui->sizeEdit->setText("0");

    if(!(parentItem->getTagType() == amc::TagType::ByteArray
         || parentItem->getTagType() == amc::TagType::IntArray
         || parentItem->getTagType() == amc::TagType::LongArray))
    {
        m_ui->sizeLabel->setEnabled(false);
        m_ui->sizeEdit->setEnabled(false);
    }

    setWindowTitle(QString("Add %1 Tag").arg(tagNames[static_cast<int>(parentItem->getTagType())]));
}

NewTagDialog::~NewTagDialog()
{
    delete m_ui;
}

QString NewTagDialog::getName() const
{
    return m_ui->nameEdit->text();
}

int NewTagDialog::getSize() const
{
    return m_ui->sizeEdit->text().toInt();
}

void NewTagDialog::accept()
{
    QString tagName = m_ui->nameEdit->text();
    if(m_parentItem->hasChildWithName(tagName)) {
        QMessageBox::information(this, windowTitle(), tr("The name already exists for this parent."));
        return;
    }

    QDialog::accept();
}

} // namespace anv