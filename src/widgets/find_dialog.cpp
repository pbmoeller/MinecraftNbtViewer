#include "find_dialog.hpp"
#include "util/iconprovider.hpp"

#include <cpp-anvil/nbt/types.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

namespace minecraft {
namespace nbt {

void addComboItem(QComboBox* comboBox, anvil::TagType tagType, IconProvider::IconSize iconSize)
{
    QString name = QString(anvil::getTagName(tagType).data());
    int data     = static_cast<int>(tagType);

    comboBox->addItem(IconProvider::icon(tagType, iconSize), name, QVariant(data));
}

FindDialog::FindDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi();
}

FindDialog::~FindDialog() = default;

SearchCriteria FindDialog::searchCriteria() const
{
    SearchCriteria criteria;
    if(m_nameCheck->isChecked() && !m_nameLineEdit->text().isEmpty()) {
        criteria.name = m_nameLineEdit->text();
    }
    if(m_valueCheck->isChecked() && !m_valueLineEdit->text().isEmpty()) {
        criteria.value = m_valueLineEdit->text();
    }
    if(m_typeCheck->isChecked()) {
        criteria.type = static_cast<anvil::TagType>(m_typeCombo->currentData().toInt());
    }
    criteria.direction =
        m_directionCheck->isChecked() ? SearchDirection::Backward : SearchDirection::Forward;
    criteria.caseSensitive = m_matchCaseCheck->isChecked();
    criteria.downOnly      = m_downwardsOnlyCheck->isChecked();
    criteria.wrapAround    = m_wrapAroundCheck->isChecked();

    return criteria;
}

void FindDialog::accept()
{
    QDialog::accept();
}

void FindDialog::anyCheckStateChanged(Qt::CheckState state)
{
    Q_UNUSED(state);
    updateSearchButtonVisibility();
}

void FindDialog::updateNameCheck(const QString& text)
{
    m_nameCheck->setChecked(!text.isEmpty());
}

void FindDialog::updateValueCheck(const QString& text)
{
    m_valueCheck->setChecked(!text.isEmpty());
}

void FindDialog::setupUi()
{
    ///// Find Box
    QGridLayout* gLayout = new QGridLayout;

    // Row #1
    m_nameCheck = new QCheckBox("Name:");
    connect(m_nameCheck, &QCheckBox::checkStateChanged, this, &FindDialog::anyCheckStateChanged);
    m_nameLineEdit = new QLineEdit();
    connect(m_nameLineEdit, &QLineEdit::textChanged, this, &FindDialog::updateNameCheck);

    gLayout->addWidget(m_nameCheck, 0, 0);
    gLayout->addWidget(m_nameLineEdit, 0, 1);

    // Row #2
    m_valueCheck = new QCheckBox("Value:");
    connect(m_valueCheck, &QCheckBox::checkStateChanged, this, &FindDialog::anyCheckStateChanged);
    m_valueLineEdit = new QLineEdit();
    connect(m_valueLineEdit, &QLineEdit::textChanged, this, &FindDialog::updateValueCheck);

    gLayout->addWidget(m_valueCheck, 1, 0);
    gLayout->addWidget(m_valueLineEdit, 1, 1);

    // Row #3
    m_typeCheck = new QCheckBox("Type:");
    connect(m_typeCheck, &QCheckBox::checkStateChanged, this, &FindDialog::anyCheckStateChanged);
    m_typeCombo = new QComboBox();
    m_typeCombo->setMinimumWidth(200);

    const IconProvider::IconSize iconSize = IconProvider::IconSize::Size16;
    addComboItem(m_typeCombo, anvil::TagType::Byte, iconSize);
    addComboItem(m_typeCombo, anvil::TagType::Short, iconSize);
    addComboItem(m_typeCombo, anvil::TagType::Int, iconSize);
    addComboItem(m_typeCombo, anvil::TagType::Long, iconSize);
    addComboItem(m_typeCombo, anvil::TagType::Float, iconSize);
    addComboItem(m_typeCombo, anvil::TagType::Double, iconSize);
    addComboItem(m_typeCombo, anvil::TagType::ByteArray, iconSize);
    addComboItem(m_typeCombo, anvil::TagType::String, iconSize);
    addComboItem(m_typeCombo, anvil::TagType::List, iconSize);
    addComboItem(m_typeCombo, anvil::TagType::Compound, iconSize);
    addComboItem(m_typeCombo, anvil::TagType::IntArray, iconSize);
    addComboItem(m_typeCombo, anvil::TagType::LongArray, iconSize);

    gLayout->addWidget(m_typeCheck, 2, 0);
    gLayout->addWidget(m_typeCombo, 2, 1);

    QGroupBox* findGroup = new QGroupBox("Find what");
    findGroup->setLayout(gLayout);

    ///// Search Options Box
    QVBoxLayout* vLayout = new QVBoxLayout;

    m_directionCheck     = new QCheckBox("Backward direction");
    m_matchCaseCheck     = new QCheckBox("Match case");
    m_wrapAroundCheck    = new QCheckBox("Wrap around");
    m_downwardsOnlyCheck = new QCheckBox("Downwards only");
    vLayout->addWidget(m_directionCheck);
    vLayout->addWidget(m_matchCaseCheck);
    vLayout->addWidget(m_wrapAroundCheck);
    vLayout->addWidget(m_downwardsOnlyCheck);

    QGroupBox* searchOptionsGroup = new QGroupBox("Search Options");
    searchOptionsGroup->setLayout(vLayout);

    ///// Search / Cancel Buttons

    m_searchButton = new QPushButton("Search");
    connect(m_searchButton, &QPushButton::clicked, this, &QDialog::accept);
    QPushButton* cancelButton = new QPushButton("Cancel");
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    QHBoxLayout* hButtonLayout = new QHBoxLayout;
    hButtonLayout->addStretch(1);
    hButtonLayout->addWidget(m_searchButton);
    hButtonLayout->addWidget(cancelButton);

    ///// Layout Assembly
    QGridLayout* gOuterLayout = new QGridLayout;
    gOuterLayout->addWidget(findGroup, 0, 0);
    gOuterLayout->addWidget(searchOptionsGroup, 0, 1, 2, 1);
    gOuterLayout->addLayout(hButtonLayout, 2, 0, 1, 2);

    setLayout(gOuterLayout);

    updateSearchButtonVisibility();
}

void FindDialog::updateSearchButtonVisibility()
{
    m_searchButton->setEnabled(m_nameCheck->isChecked() || m_valueCheck->isChecked()
                               || m_typeCheck->isChecked());
}

} // namespace nbt
} // namespace minecraft
