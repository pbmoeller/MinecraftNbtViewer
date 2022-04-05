// AwesomeNbtViewer
#include "edit_dialog.hpp"
#include "models/nbt_data_treemodel.hpp"
#include "models/treeitems/nbt_treeitem_nbttag.hpp"
#include "util/iconprovider.hpp"
#include "util/validators/int8_validator.hpp"
#include "util/validators/int16_validator.hpp"
#include "util/validators/int32_validator.hpp"
#include "util/validators/int64_validator.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/tags/tags.hpp>

// Qt
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

namespace anv
{

QValidator* createValidator(amc::TagType tagType)
{
    if(tagType == amc::TagType::Byte) {
        return new Int8Validator();
    } else if(tagType == amc::TagType::Short) {
        return new Int16Validator();
    } else if(tagType == amc::TagType::Int) {
        return new Int32Validator();
    } else if(tagType == amc::TagType::Long) {
        return new Int64Validator();
    }
    
    return nullptr;
}

EditDialog::EditDialog(NbtTreeItemNbtTag *treeItem,
                       NbtDataTreeModel *model,
                       EditFunction function,
                       QWidget *parent)
    : QDialog(parent)
    , m_treeItem(treeItem)
    , m_model(model)
    , m_hasRenameField(false)
    , m_hasEditField(false)
    , m_lineEditName(nullptr)
    , m_lineEditValue(nullptr)
    , m_textEdit(nullptr)
{
    setupUi(function);
}

EditDialog::~EditDialog()
{

}

void EditDialog::accept()
{
    bool dataChanged = false;

    if(m_hasRenameField) {
        if(m_treeItem->getName() == m_lineEditName->text()) {
            qDebug() << "Name didn't change";
        } else {
            // TODO: Check if siblings have the same name?!? 
            //    - Except in Lists -> all items have no name
            qDebug() << "Name changed";
            m_treeItem->rename(m_lineEditName->text());
            dataChanged = true;
        }
    }
    if(m_hasEditField) {

    }

    if(dataChanged) {
        m_model->itemChanged(m_treeItem);
    }
    QDialog::accept();
}

void EditDialog::setupUi(EditFunction function)
{
    int currentRow          = 0;
    m_hasRenameField        = m_treeItem->canRename();
    m_hasEditField          = m_treeItem->canEdit();
    amc::TagType tagType    = m_treeItem->getTag()->getType();

    QGridLayout *gLayout = new QGridLayout(this);

    ///////    Row #1
    if(m_hasRenameField) {
        QLabel *labelName = new QLabel(tr("Name:"));
        m_lineEditName = new QLineEdit();
        m_lineEditName->setText(m_treeItem->getTag()->getName().c_str());

        gLayout->addWidget(labelName, currentRow, 0);
        gLayout->addWidget(m_lineEditName, currentRow, 1);

        ++currentRow;
    }

    ///////    Row #2
    if(m_hasEditField) {
        QLabel *labelValue = new QLabel(tr("Value:"));
        gLayout->addWidget(labelValue, currentRow, 0);

        if(tagType == amc::TagType::String) {
            m_textEdit = new QTextEdit();
            m_textEdit->setText(valueToString());

            gLayout->addWidget(m_textEdit, currentRow, 1);
        } else if(amc::isValueTag(tagType)) {
            m_lineEditValue = new QLineEdit();
            m_lineEditValue->setText(valueToString());
            m_lineEditValue->setValidator(createValidator(tagType));

            gLayout->addWidget(m_lineEditValue, currentRow, 1);
        } else if(amc::isArrayTag(tagType)) {
            gLayout->addWidget(new QLabel("Array Tag -> TBD"), currentRow, 1);
        } else if(amc::isContainerTag(tagType)) {
            gLayout->addWidget(new QLabel("List/CompoundTag Tag -> to be left out"), currentRow, 1);
        }

        ++currentRow;
    }

    // Last Row
    QPushButton *buttonCancel = new QPushButton(tr("Cancel"));
    buttonCancel->setAutoDefault(false);
    QPushButton *buttonOk = new QPushButton(tr("Ok"));
    buttonOk->setDefault(true);
    buttonOk->setAutoDefault(true);

    connect(buttonCancel,   &QPushButton::clicked, this, &QDialog::reject);
    connect(buttonOk,       &QPushButton::clicked, this, &QDialog::accept);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(buttonCancel);
    hLayout->addWidget(buttonOk);

    gLayout->addLayout(hLayout, currentRow, 0, 1, 2);

    // Set Layout
    setLayout(gLayout);

    // Set Selection (it seems that it must be called after the Layout is set)
    if(m_hasRenameField && function == EditFunction::Rename) {
        m_lineEditName->setFocus(Qt::OtherFocusReason);
        m_lineEditName->selectAll();
    } else if(m_hasEditField && function == EditFunction::EditValue) {
        if(tagType == amc::TagType::String) {
            m_textEdit->setFocus(Qt::OtherFocusReason);
            m_textEdit->selectAll();
        } else if(amc::isValueTag(tagType)) {
            m_lineEditValue->setFocus(Qt::OtherFocusReason);
            m_lineEditValue->selectAll();
        } else if(amc::isContainerTag(tagType)) {
            m_lineEditName->setFocus(Qt::OtherFocusReason);
            m_lineEditName->selectAll();
        }
    }

    // Set WindowTitle and Icon
    setWindowTitle(tr("Edit %1 value").arg(amc::getTagName(tagType).c_str()));
    setWindowIcon(IconProvider::icon(tagType, IconProvider::Size16));
}

QString EditDialog::valueToString() const
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

} // namespace anv