// AwesomeNbtViewer
#include "edit_dialog.hpp"
#include "models/nbt_data_treemodel.hpp"
#include "models/treeitems/nbt_treeitem_nbttag.hpp"
#include "util/iconprovider.hpp"
#include "util/validators/float_validator.hpp"
#include "util/validators/int8_validator.hpp"
#include "util/validators/int16_validator.hpp"
#include "util/validators/int32_validator.hpp"
#include "util/validators/int64_validator.hpp"

// AwesomeMC
#include <AwesomeMC/nbt/tags/tags.hpp>

// Qt
#include <QByteArray>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QMessageBox>
#include <QRegularExpression>

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
    } else if(tagType == amc::TagType::Float) {
        return new FloatValidator();
    } else if(tagType == amc::TagType::Double) {
        return new QDoubleValidator();
    } else {

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

    // Check the name if present
    if(m_hasRenameField) {
        QString newName = m_lineEditName->text();
        if(m_treeItem->getName() != newName) {
            // Check if there is already a sibling with the same name
            NbtTreeItemBase *parentTreeItem = m_treeItem->getParent();
            for(NbtTreeItemBase *child : parentTreeItem->getChildren()) {
                if(newName == child->getName()) {
                    QMessageBox::information(this, windowTitle(), tr("There is already a sibling with this name. Please choose a diffrent."));
                    m_lineEditName->setFocus(Qt::OtherFocusReason);
                    m_lineEditName->selectAll();
                    return;
                }
            }

            qDebug() << "Name changed";
            m_treeItem->rename(m_lineEditName->text());
            dataChanged = true;
        }
    }

    // Check the value if present
    if(m_hasEditField) {
        amc::TagType tagType = m_treeItem->getTag()->getType();
        if(tagType == amc::TagType::String) {
            QString value = m_textEdit->toPlainText();
            amc::StringTag *stringTag = tag_cast<amc::StringTag*>(m_treeItem->getTag());
            if(stringTag && value.toStdString() != stringTag->getValue()) {
                stringTag->setValue(value.toStdString());
                dataChanged = true;
            }
        } else if(amc::isValueTag(tagType)) {
            QString value = m_lineEditValue->text();

            dataChanged = checkAndSetValue(value, m_treeItem->getTag());
        } else if(amc::isArrayTag(tagType)) {
            QString value = m_textEdit->toPlainText();

            dataChanged = checkAndSetArrayValue(value, m_treeItem->getTag());
        }
    }

    // Check if data has changed and we need to notify the model
    if(dataChanged) {
        m_model->itemChanged(m_treeItem);
    }
    QDialog::accept();
}

void EditDialog::setupUi(EditFunction function)
{
    int currentRow          = 0;
    amc::TagType tagType    = m_treeItem->getTag()->getType();
    m_hasRenameField        = m_treeItem->canRename();
    m_hasEditField          = m_treeItem->canEdit() && !amc::isContainerTag(tagType);

    QGridLayout *gLayout = new QGridLayout(this);

    ///////    Row #1
    if(m_hasRenameField) {
        QLabel *labelName = new QLabel(tr("Name:"));
        labelName->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        m_lineEditName = new QLineEdit();
        m_lineEditName->setText(m_treeItem->getTag()->getName().c_str());

        gLayout->addWidget(labelName, currentRow, 0);
        gLayout->addWidget(m_lineEditName, currentRow, 1);

        ++currentRow;
    }

    ///////    Row #2
    if(m_hasEditField) {
        QLabel *labelValue = new QLabel(tr("Value:"));
        labelValue->setAlignment(Qt::AlignLeft | Qt::AlignTop);
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
            m_textEdit = new QTextEdit();
            m_textEdit->setText(arrayToString());

            gLayout->addWidget(m_textEdit, currentRow, 1);
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
    hLayout->addWidget(buttonOk);
    hLayout->addWidget(buttonCancel);

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

    // Set thw window size.
    QSize size = sizeHint();
    if(m_lineEditValue) {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setFixedSize(size + QSize(50, 0));
    } else if(m_textEdit) {
        setSizeGripEnabled(true);
        resize(size + QSize(100, 0));
    }
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

QString EditDialog::arrayToString() const
{
    QByteArray out;
    switch(m_treeItem->getTag()->getType()) {
        case amc::TagType::ByteArray:
        {
            int numbersPerLine = 16;
            amc::ByteArrayTag *byteArray = tag_cast<amc::ByteArrayTag*>(m_treeItem->getTag());
            for(int i = 0; i < byteArray->size(); ++i) {
                out += QByteArray::number(static_cast<int>((*byteArray)[i]), 10) + "  ";

                if((i + 1) % numbersPerLine == 0) {
                    out += "\n";
                }
            }

            break;
        }
        case amc::TagType::IntArray:
        {
            int numbersPerLine = 4;
            amc::IntArrayTag *byteArray = tag_cast<amc::IntArrayTag*>(m_treeItem->getTag());
            for(int i = 0; i < byteArray->size(); ++i) {
                out += QByteArray::number(static_cast<int>(((*byteArray)[i])), 10) + "  ";

                if((i + 1) % numbersPerLine == 0) {
                    out += "\n";
                }
            }

            break;
        }
        case amc::TagType::LongArray:
        {
            int numbersPerLine = 2;
            amc::LongArrayTag *byteArray = tag_cast<amc::LongArrayTag*>(m_treeItem->getTag());
            for(int i = 0; i < byteArray->size(); ++i) {
                out += QByteArray::number(static_cast<int>((*byteArray)[i]), 10) + "  ";

                if((i + 1) % numbersPerLine == 0) {
                    out += "\n";
                }
            }

            break;
        }
    }

    return out;
}

bool EditDialog::checkAndSetValue(const QString &value, amc::AbstractTag *tag)
{
    bool ok = false;
    bool dataChanged = false;
    switch(tag->getType()) {
        case amc::TagType::Byte:
        {
            int8_t newValue = static_cast<int8_t>(value.toInt(&ok));
            amc::ByteTag *byteTag = amc::tag_cast<amc::ByteTag*>(tag);
            if(ok && byteTag->getValue() != newValue) {
                byteTag->setValue(newValue);
                dataChanged = true;
            }
            break;
        }
        case amc::TagType::Short:
        {
            int16_t newValue = value.toShort(&ok);
            amc::ShortTag *shortTag = amc::tag_cast<amc::ShortTag*>(tag);
            if(ok && shortTag->getValue() != newValue) {
                shortTag->setValue(newValue);
                dataChanged = true;
            }
            break;
        }
        case amc::TagType::Int:
        {
            int32_t newValue = value.toInt(&ok);
            amc::IntTag *intTag = amc::tag_cast<amc::IntTag*>(tag);
            if(ok && intTag->getValue() != newValue) {
                intTag->setValue(newValue);
                dataChanged = true;
            }
            break;
        }
        case amc::TagType::Long:
        {
            int64_t newValue = value.toLongLong(&ok);
            amc::LongTag *longTag = amc::tag_cast<amc::LongTag*>(tag);
            if(ok && longTag->getValue() != newValue) {
                longTag->setValue(newValue);
                dataChanged = true;
            }
            break;
        }
        case amc::TagType::Float:
        {
            float newValue = value.toFloat(&ok);
            amc::FloatTag *floatTag = amc::tag_cast<amc::FloatTag*>(tag);
            if(ok && floatTag->getValue() != newValue) {
                floatTag->setValue(newValue);
                dataChanged = true;
            }
            break;
        }
        case amc::TagType::Double:
        {
            double newValue = value.toDouble(&ok);
            amc::DoubleTag *doubleTag = amc::tag_cast<amc::DoubleTag*>(tag);
            if(ok && doubleTag->getValue() != newValue) {
                doubleTag->setValue(newValue);
                dataChanged = true;
            }
            break;
        }
    }

    if(!ok) {
        QMessageBox::warning(this, windowTitle(), tr("Error converting the data"));
    }

    return dataChanged;
}

bool EditDialog::checkAndSetArrayValue(const QString &value, amc::AbstractTag *tag)
{
    bool dataChanged = false;
    bool setNewValues = true;
    bool oneOrMoreInvalidValues = false;
    switch(tag->getType()) {
        case amc::TagType::ByteArray:
        {
            amc::ByteArrayTag *arrTag = dynamic_cast<amc::ByteArrayTag*>(tag);
            if(!arrTag) {
                return false;
            }

            std::vector<int8_t> byteArray;
            for(const QString &str : value.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts)) {
                bool ok;
                int32_t nextValue = str.toInt(&ok);
                if(!ok || (nextValue < std::numeric_limits<int8_t>::min() || nextValue > std::numeric_limits<int8_t>::max())) {
                    nextValue = 0;
                    oneOrMoreInvalidValues = true;
                }
                byteArray.push_back(nextValue);
            }

            if(oneOrMoreInvalidValues) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, windowTitle(), tr("There are one or more invalid values, that will be set to 0.\nDo you want to apply these changes?"),
                                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if(reply == QMessageBox::No) {
                    setNewValues = false;
                }
            }

            if(setNewValues && arrTag->getValue() != byteArray) {
                arrTag->setValue(byteArray);
                dataChanged = true;
            }

            break;
        }
        case amc::TagType::IntArray:
        {
            amc::IntArrayTag *arrTag = dynamic_cast<amc::IntArrayTag*>(tag);
            if(!arrTag) {
                return false;
            }

            std::vector<int32_t> intArray;
            for(const QString &str : value.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts)) {
                bool ok;
                int32_t nextValue = str.toInt(&ok);
                if(!ok) {
                    nextValue = 0;
                    oneOrMoreInvalidValues = true;
                }
                intArray.push_back(nextValue);
            }

            if(oneOrMoreInvalidValues) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, windowTitle(), tr("There are one or more invalid values, that will be set to 0.\nDo you want to apply these changes?"),
                                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if(reply == QMessageBox::No) {
                    setNewValues = false;
                }
            }

            if(setNewValues && arrTag->getValue() != intArray) {
                arrTag->setValue(intArray);
                dataChanged = true;
            }

            break;
        }
        case amc::TagType::LongArray:
        {
            amc::LongArrayTag *arrTag = dynamic_cast<amc::LongArrayTag*>(tag);
            if(!arrTag) {
                return false;
            }

            std::vector<int64_t> longArray;
            for(const QString &str : value.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts)) {
                bool ok;
                int64_t nextValue = str.toLongLong(&ok);
                if(!ok) {
                    nextValue = 0;
                    oneOrMoreInvalidValues = true;
                }
                longArray.push_back(nextValue);
            }

            if(oneOrMoreInvalidValues) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, windowTitle(), tr("There are one or more invalid values, that will be set to 0.\nDo you want to apply these changes?"),
                                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if(reply == QMessageBox::No) {
                    setNewValues = false;
                }
            }

            if(setNewValues && arrTag->getValue() != longArray) {
                arrTag->setValue(longArray);
                dataChanged = true;
            }

            break;
        }
    }

    return dataChanged;
}

} // namespace anv