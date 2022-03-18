// AwesomeNbtViewer
#include "edit_value_dialog.hpp"
#include "ui_edit_value_dialog.h"

// AwesomeMC
#include <AwesomeMC/nbt/tags/tag_type.hpp>
#include <AwesomeMC/nbt/tags/tags.hpp>

// Qt
#include <QMessageBox>

// STL
#include <climits>

namespace anv
{

EditValueDialog::EditValueDialog(NbtTreeItemNbtTag *treeItem, NbtDataTreeModel *model, QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::EditValueDialog)
    , m_treeItem(treeItem)
    , m_model(model)
{
    m_ui->setupUi(this);

    setWindowTitle(tr("Edit %1 value").arg(amc::getTagName(m_treeItem->getTag()->getType()).data()));
    m_ui->valueEdit->setText(valueToString());
    m_ui->valueEdit->selectAll();
}

EditValueDialog::~EditValueDialog()
{
    delete m_ui;
}

QString EditValueDialog::valueToString()
{
    switch(m_treeItem->getTag()->getType()) {
        case amc::TagType::Byte:
            return QString::number(tag_cast<amc::ByteTag*>(m_treeItem->getTag())->getValue());
        case amc::TagType::Short:
            return QString::number(tag_cast<amc::ShortTag*>(m_treeItem->getTag())->getValue());
        case amc::TagType::Int:
            return QString::number(tag_cast<amc::IntTag*>(m_treeItem->getTag())->getValue());
        case amc::TagType::Long:
            return QString::number(tag_cast<amc::LongTag*>(m_treeItem->getTag())->getValue());
        case amc::TagType::Float:
            return QString::number(tag_cast<amc::FloatTag*>(m_treeItem->getTag())->getValue(), 'g', 6);
        case amc::TagType::Double:
            return QString::number(tag_cast<amc::DoubleTag*>(m_treeItem->getTag())->getValue(), 'g', 16);
        case amc::TagType::String:
            return QString(tag_cast<amc::StringTag*>(m_treeItem->getTag())->getValue().c_str());
        default:
            break;
    }
    return QString();
}

void EditValueDialog::accept()
{
    QString tagValue = m_ui->valueEdit->text();
    bool ok;
    switch(m_treeItem->getTag()->getType()) {
        case amc::TagType::Byte:
        {
            int16_t newValue = tagValue.toShort(&ok);
            if(ok && (newValue >= std::numeric_limits<int8_t>::min() 
                      && newValue <= std::numeric_limits<int8_t>::max())) {
                amc::tag_cast<amc::ByteTag*>(m_treeItem->getTag())->setValue(newValue);
            } else {
                QMessageBox::information(this, windowTitle(), tr("Can not convert value to byte."));
                return;
            }
            break;
        }
        case amc::TagType::Short:
        {
            int16_t newValue = tagValue.toShort(&ok);
            if(ok) {
                amc::tag_cast<amc::ShortTag*>(m_treeItem->getTag())->setValue(newValue);
            } else {
                QMessageBox::information(this, windowTitle(), tr("Can not convert value to short."));
                return;
            }
            break;
        }
        case amc::TagType::Int:
        {
            int32_t newValue = tagValue.toInt(&ok);
            if(ok) {
                amc::tag_cast<amc::IntTag*>(m_treeItem->getTag())->setValue(newValue);
            } else {
                QMessageBox::information(this, windowTitle(), tr("Can not convert value to int."));
                return;
            }
            break;
        }
        case amc::TagType::Long:
        {
            int64_t newValue = tagValue.toLongLong(&ok);
            if(ok) {
                amc::tag_cast<amc::LongTag*>(m_treeItem->getTag())->setValue(newValue);
            } else {
                QMessageBox::information(this, windowTitle(), tr("Can not convert value to long."));
                return;
            }
            break;
        }
        case amc::TagType::Float:
        {
            float newValue = tagValue.toFloat(&ok);
            if(ok) {
                amc::tag_cast<amc::FloatTag*>(m_treeItem->getTag())->setValue(newValue);
            } else {
                QMessageBox::information(this, windowTitle(), tr("Can not convert value to float."));
                return;
            }
            break;
        }
        case amc::TagType::Double:
        {
            double newValue = tagValue.toDouble(&ok);
            if(ok) {
                amc::tag_cast<amc::DoubleTag*>(m_treeItem->getTag())->setValue(newValue);
            } else {
                QMessageBox::information(this, windowTitle(), tr("Can not convert value to double."));
                return;
            }
            break;
        }
        case amc::TagType::String:
            amc::tag_cast<amc::StringTag*>(m_treeItem->getTag())->setValue(tagValue.toStdString());
            break;
        default:
            break;
    }

    m_model->itemChanged(m_treeItem);
    QDialog::accept();
}

} // namespace anv