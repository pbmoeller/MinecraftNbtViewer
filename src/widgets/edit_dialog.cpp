// MinecraftNbtViewer
#include "edit_dialog.hpp"
#include "models/nbt_data_treemodel.hpp"
#include "models/treeitems/nbt_treeitem_nbttag.hpp"
#include "util/iconprovider.hpp"
#include "util/validators/float_validator.hpp"
#include "util/validators/int8_validator.hpp"
#include "util/validators/int16_validator.hpp"
#include "util/validators/int32_validator.hpp"
#include "util/validators/int64_validator.hpp"

// cpp-anvil
#include <cpp-anvil/nbt.hpp>

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

namespace minecraft {
namespace nbt {

QValidator* createValidator(anvil::TagType tagType)
{
    if(tagType == anvil::TagType::Byte) {
        return new Int8Validator();
    } else if(tagType == anvil::TagType::Short) {
        return new Int16Validator();
    } else if(tagType == anvil::TagType::Int) {
        return new Int32Validator();
    } else if(tagType == anvil::TagType::Long) {
        return new Int64Validator();
    } else if(tagType == anvil::TagType::Float) {
        return new FloatValidator();
    } else if(tagType == anvil::TagType::Double) {
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

EditDialog::~EditDialog() = default;

void EditDialog::accept()
{
    bool nameChanged = false;
    bool dataChanged = false;

    // Check the name if present
    if(m_hasRenameField) {
        QString newName = m_lineEditName->text();
        if(m_treeItem->name() != newName) {
            // Check if there is already a sibling with the same name
            NbtTreeItemBase *parentTreeItem = m_treeItem->parent();
            for(NbtTreeItemBase *child : parentTreeItem->children()) {
                if(newName == child->name()) {
                    QMessageBox::information(this, windowTitle(), tr("There is already a sibling with this name. Please choose a diffrent."));
                    m_lineEditName->setFocus(Qt::OtherFocusReason);
                    m_lineEditName->selectAll();
                    return;
                }
            }

            qDebug() << "Name changed";
            m_treeItem->rename(m_lineEditName->text());
            nameChanged = true;
        }
    }

    // Check the value if present
    if(m_hasEditField) {
        anvil::TagType tagType = m_treeItem->tag()->type();
        if(tagType == anvil::TagType::String) {
            QString value = m_textEdit->toPlainText();
            anvil::StringTag *stringTag = tag_cast<anvil::StringTag*>(m_treeItem->tag());
            if(stringTag && value.toStdString() != stringTag->value()) {
                stringTag->setValue(value.toStdString());
                dataChanged = true;
            }
        } else if(anvil::isValueTag(tagType)) {
            QString value = m_lineEditValue->text();

            dataChanged = checkAndSetValue(value, m_treeItem->tag());
        } else if(anvil::isArrayTag(tagType)) {
            QString value = m_textEdit->toPlainText();

            dataChanged = checkAndSetArrayValue(value, m_treeItem->tag());
        }
    }

    // Check if data has changed and we need to notify the model
    if(nameChanged || dataChanged) {
        m_model->itemChanged(m_treeItem);
    }
    QDialog::accept();
}

void EditDialog::setupUi(EditFunction function)
{
    int currentRow          = 0;
    anvil::TagType tagType    = m_treeItem->tag()->type();
    m_hasRenameField        = m_treeItem->canRename();
    m_hasEditField          = m_treeItem->canEdit() && !anvil::isContainerTag(tagType);

    QGridLayout *gLayout = new QGridLayout(this);

    ///////    Row #1
    if(m_hasRenameField) {
        QLabel *labelName = new QLabel(tr("Name:"));
        labelName->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        m_lineEditName = new QLineEdit();
        m_lineEditName->setText(m_treeItem->tag()->name().c_str());

        gLayout->addWidget(labelName, currentRow, 0);
        gLayout->addWidget(m_lineEditName, currentRow, 1);

        ++currentRow;
    }

    ///////    Row #2
    if(m_hasEditField) {
        QLabel *labelValue = new QLabel(tr("Value:"));
        labelValue->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        gLayout->addWidget(labelValue, currentRow, 0);

        if(tagType == anvil::TagType::String) {
            m_textEdit = new QTextEdit();
            m_textEdit->setText(valueToString());

            gLayout->addWidget(m_textEdit, currentRow, 1);
        } else if(anvil::isValueTag(tagType)) {
            m_lineEditValue = new QLineEdit();
            m_lineEditValue->setText(valueToString());
            m_lineEditValue->setValidator(createValidator(tagType));

            gLayout->addWidget(m_lineEditValue, currentRow, 1);
        } else if(anvil::isArrayTag(tagType)) {
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
        if(tagType == anvil::TagType::String) {
            m_textEdit->setFocus(Qt::OtherFocusReason);
            m_textEdit->selectAll();
        } else if(anvil::isValueTag(tagType)) {
            m_lineEditValue->setFocus(Qt::OtherFocusReason);
            m_lineEditValue->selectAll();
        } else if(anvil::isContainerTag(tagType)) {
            m_lineEditName->setFocus(Qt::OtherFocusReason);
            m_lineEditName->selectAll();
        }
    }

    // Set WindowTitle and Icon
    setWindowTitle(tr("Edit %1 value").arg(anvil::getTagName(tagType).data()));
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
    switch(m_treeItem->tag()->type()) {
        case anvil::TagType::Byte:
            return QString::number(tag_cast<anvil::ByteTag*>(m_treeItem->tag())->value());
        case anvil::TagType::Short:
            return QString::number(tag_cast<anvil::ShortTag*>(m_treeItem->tag())->value());
        case anvil::TagType::Int:
            return QString::number(tag_cast<anvil::IntTag*>(m_treeItem->tag())->value());
        case anvil::TagType::Long:
            return QString::number(tag_cast<anvil::LongTag*>(m_treeItem->tag())->value());
        case anvil::TagType::Float:
            return QString::number(tag_cast<anvil::FloatTag*>(m_treeItem->tag())->value(), 'g', 6);
        case anvil::TagType::Double:
            return QString::number(tag_cast<anvil::DoubleTag*>(m_treeItem->tag())->value(), 'g', 16);
        case anvil::TagType::String:
            return QString(tag_cast<anvil::StringTag*>(m_treeItem->tag())->value().c_str());
        default:
            break;
    }
    return QString();
}

QString EditDialog::arrayToString() const
{
    QByteArray out;
    switch(m_treeItem->tag()->type()) {
        case anvil::TagType::ByteArray:
        {
            int numbersPerLine = 16;
            anvil::ByteArrayTag *byteArray = tag_cast<anvil::ByteArrayTag*>(m_treeItem->tag());
            for(int i = 0; i < byteArray->size(); ++i) {
                out += QByteArray::number(static_cast<int>((*byteArray)[i]), 10) + "  ";

                if((i + 1) % numbersPerLine == 0) {
                    out += "\n";
                }
            }

            break;
        }
        case anvil::TagType::IntArray:
        {
            int numbersPerLine = 4;
            anvil::IntArrayTag *byteArray = tag_cast<anvil::IntArrayTag*>(m_treeItem->tag());
            for(int i = 0; i < byteArray->size(); ++i) {
                out += QByteArray::number(static_cast<int>(((*byteArray)[i])), 10) + "  ";

                if((i + 1) % numbersPerLine == 0) {
                    out += "\n";
                }
            }

            break;
        }
        case anvil::TagType::LongArray:
        {
            int numbersPerLine = 2;
            anvil::LongArrayTag *byteArray = tag_cast<anvil::LongArrayTag*>(m_treeItem->tag());
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

bool EditDialog::checkAndSetValue(const QString &value, anvil::BasicTag *tag)
{
    bool ok = false;
    bool dataChanged = false;
    switch(tag->type()) {
        case anvil::TagType::Byte:
        {
            int8_t newValue = static_cast<int8_t>(value.toInt(&ok));
            anvil::ByteTag *byteTag = anvil::tag_cast<anvil::ByteTag*>(tag);
            if(ok && byteTag->value() != newValue) {
                byteTag->setValue(newValue);
                dataChanged = true;
            }
            break;
        }
        case anvil::TagType::Short:
        {
            int16_t newValue = value.toShort(&ok);
            anvil::ShortTag *shortTag = anvil::tag_cast<anvil::ShortTag*>(tag);
            if(ok && shortTag->value() != newValue) {
                shortTag->setValue(newValue);
                dataChanged = true;
            }
            break;
        }
        case anvil::TagType::Int:
        {
            int32_t newValue = value.toInt(&ok);
            anvil::IntTag *intTag = anvil::tag_cast<anvil::IntTag*>(tag);
            if(ok && intTag->value() != newValue) {
                intTag->setValue(newValue);
                dataChanged = true;
            }
            break;
        }
        case anvil::TagType::Long:
        {
            int64_t newValue = value.toLongLong(&ok);
            anvil::LongTag *longTag = anvil::tag_cast<anvil::LongTag*>(tag);
            if(ok && longTag->value() != newValue) {
                longTag->setValue(newValue);
                dataChanged = true;
            }
            break;
        }
        case anvil::TagType::Float:
        {
            float newValue = value.toFloat(&ok);
            anvil::FloatTag *floatTag = anvil::tag_cast<anvil::FloatTag*>(tag);
            if(ok && floatTag->value() != newValue) {
                floatTag->setValue(newValue);
                dataChanged = true;
            }
            break;
        }
        case anvil::TagType::Double:
        {
            double newValue = value.toDouble(&ok);
            anvil::DoubleTag *doubleTag = anvil::tag_cast<anvil::DoubleTag*>(tag);
            if(ok && doubleTag->value() != newValue) {
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

bool EditDialog::checkAndSetArrayValue(const QString &value, anvil::BasicTag *tag)
{
    bool dataChanged = false;
    bool setNewValues = true;
    bool oneOrMoreInvalidValues = false;
    switch(tag->type()) {
        case anvil::TagType::ByteArray:
        {
            anvil::ByteArrayTag *arrTag = dynamic_cast<anvil::ByteArrayTag*>(tag);
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

            if(setNewValues && arrTag->value() != byteArray) {
                arrTag->setValue(byteArray);
                dataChanged = true;
            }

            break;
        }
        case anvil::TagType::IntArray:
        {
            anvil::IntArrayTag *arrTag = dynamic_cast<anvil::IntArrayTag*>(tag);
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

            if(setNewValues && arrTag->value() != intArray) {
                arrTag->setValue(intArray);
                dataChanged = true;
            }

            break;
        }
        case anvil::TagType::LongArray:
        {
            anvil::LongArrayTag *arrTag = dynamic_cast<anvil::LongArrayTag*>(tag);
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

            if(setNewValues && arrTag->value() != longArray) {
                arrTag->setValue(longArray);
                dataChanged = true;
            }

            break;
        }
    }

    return dataChanged;
}

} // namespace nbt
} // namespace minecraft
