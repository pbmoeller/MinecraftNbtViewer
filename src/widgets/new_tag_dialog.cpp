// MinecraftNbtViewer
#include "new_tag_dialog.hpp"
#include "ui_new_tag_dialog.h"
#include "util/iconprovider.hpp"

// cpp-anvil
#include <cpp-anvil/nbt/types.hpp>

// Qt
#include <QMessageBox>
#include <QIntValidator>

// STL
#include <climits>

namespace minecraft {
namespace nbt {

NewTagDialog::NewTagDialog(NbtTreeItemNbtTag *parentItem,
                           anvil::TagType newItemTagType,
                           QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::NewTagDialog)
    , m_parentItem(parentItem)
{
    m_ui->setupUi(this);
    m_ui->sizeEdit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), this));
    m_ui->sizeEdit->setText("0");

    if(!(newItemTagType == anvil::TagType::ByteArray
         || newItemTagType == anvil::TagType::IntArray
         || newItemTagType == anvil::TagType::LongArray))
    {
        m_ui->sizeLabel->setEnabled(false);
        m_ui->sizeEdit->setEnabled(false);
    }

    setWindowIcon(IconProvider::icon(newItemTagType, IconProvider::Size16));
    setWindowTitle(QString("Add %1").arg(anvil::getTagName(newItemTagType).data()));
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

} // namespace nbt
} // namespace minecraft
