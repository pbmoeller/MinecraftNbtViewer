#include "find_dialog.hpp"
#include "util/iconprovider.hpp"

#include <cpp-anvil/nbt/types.hpp>

#include <QBoxLayout>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

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
    updateSearchButtonVisibility();
    updateFocus();
}

FindDialog::FindDialog(const SearchCriteria& criteria, QWidget* parent)
    : QDialog(parent)
{
    setupUi();
    setSearchCriteria(criteria);
    updateSearchButtonVisibility();
    updateFocus();
}

FindDialog::~FindDialog() = default;

SearchCriteria FindDialog::searchCriteria() const
{
    SearchCriteria criteria;
    criteria.name       = m_nameLineEdit->text();
    criteria.isFindName = m_nameCheck->isChecked();

    criteria.value       = m_valueLineEdit->text();
    criteria.isFindValue = m_typeCheck->isChecked();

    criteria.type = static_cast<anvil::TagType>(m_typeCombo->currentData().toInt());

    int directionRadioValue = m_directionRadioButtons->checkedId();
    if(directionRadioValue != -1) {
        criteria.direction = static_cast<SearchDirection>(directionRadioValue);
    }

    criteria.subtreeOnly = m_subtreeOnlyCheck->isChecked();
    criteria.fetchMore   = m_fetchMore->isChecked();

    // Set Match Flags
    Qt::MatchFlags matchFlags{Qt::MatchExactly};
    int matchRadioValue = m_matchRadioButtons->checkedId();
    if(matchRadioValue != -1) {
        matchFlags = static_cast<Qt::MatchFlags>(matchRadioValue);
    }

    if(m_matchCaseCheck->isChecked()) {
        matchFlags |= Qt::MatchCaseSensitive;
    }
    if(m_wrapAroundCheck->isChecked()) {
        matchFlags |= Qt::MatchWrap;
    }
    criteria.matchFlags = matchFlags;

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

void FindDialog::clear()
{
    SearchCriteria criteria;
    setSearchCriteria(criteria);
}

void FindDialog::setSearchCriteria(const SearchCriteria& criteria)
{
    m_nameCheck->setChecked(criteria.isFindName);
    m_nameLineEdit->setText(criteria.name);
    m_valueCheck->setChecked(criteria.isFindValue);
    m_valueLineEdit->setText(criteria.value);
    m_typeCombo->setCurrentIndex(m_typeCombo->findData(static_cast<int>(criteria.type)));
    m_typeCheck->setChecked(criteria.isFindType);

    m_forwardRadio->setChecked(criteria.direction == SearchDirection::Forward);
    m_backwardRadio->setChecked(criteria.direction == SearchDirection::Backward);
    m_matchCaseCheck->setChecked(criteria.matchFlags & Qt::MatchCaseSensitive);
    m_wrapAroundCheck->setChecked(criteria.matchFlags & Qt::MatchWrap);
    m_subtreeOnlyCheck->setChecked(criteria.subtreeOnly);
    m_fetchMore->setChecked(criteria.fetchMore);

    uint matchType = criteria.matchFlags & 0x0F;
    m_matchExactlyRadio->setChecked(matchType == Qt::MatchExactly);
    m_matchContainsRadio->setChecked(matchType == Qt::MatchContains);
    m_matchStartsWithRadio->setChecked(matchType == Qt::MatchStartsWith);
    m_matchEndsWithRadio->setChecked(matchType == Qt::MatchEndsWith);
}

void FindDialog::setupUi()
{
    // Widgets
    m_nameCheck = new QCheckBox("Name:");
    connect(m_nameCheck, &QCheckBox::checkStateChanged, this, &FindDialog::anyCheckStateChanged);
    m_nameLineEdit = new QLineEdit();
    connect(m_nameLineEdit, &QLineEdit::textChanged, this, &FindDialog::updateNameCheck);

    m_valueCheck = new QCheckBox("Value:");
    connect(m_valueCheck, &QCheckBox::checkStateChanged, this, &FindDialog::anyCheckStateChanged);
    m_valueLineEdit = new QLineEdit();
    connect(m_valueLineEdit, &QLineEdit::textChanged, this, &FindDialog::updateValueCheck);

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

    m_matchExactlyRadio    = new QRadioButton("Match Exactly");
    m_matchContainsRadio   = new QRadioButton("Contains");
    m_matchStartsWithRadio = new QRadioButton("Starts With");
    m_matchEndsWithRadio   = new QRadioButton("Ends With");

    m_matchRadioButtons = new QButtonGroup(this);
    m_matchRadioButtons->addButton(m_matchExactlyRadio, Qt::MatchExactly);
    m_matchRadioButtons->addButton(m_matchContainsRadio, Qt::MatchContains);
    m_matchRadioButtons->addButton(m_matchStartsWithRadio, Qt::MatchStartsWith);
    m_matchRadioButtons->addButton(m_matchEndsWithRadio, Qt::MatchEndsWith);

    m_forwardRadio  = new QRadioButton("Forward");
    m_backwardRadio = new QRadioButton("Backward");

    m_directionRadioButtons = new QButtonGroup(this);
    m_directionRadioButtons->addButton(m_forwardRadio, static_cast<int>(SearchDirection::Forward));
    m_directionRadioButtons->addButton(m_backwardRadio,
                                       static_cast<int>(SearchDirection::Backward));

    m_matchCaseCheck   = new QCheckBox("Case Sensitive");
    m_wrapAroundCheck  = new QCheckBox("Wrap around");
    m_subtreeOnlyCheck = new QCheckBox("Subtree Only");
    m_subtreeOnlyCheck->setDisabled(true); // Disable for now, as it is not implemented
    m_fetchMore        = new QCheckBox("Fetch More");

    m_searchButton = new QPushButton("Search");
    connect(m_searchButton, &QPushButton::clicked, this, &QDialog::accept);
    QPushButton* cancelButton = new QPushButton("Cancel");
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    QPushButton* clearButton = new QPushButton("Clear");
    connect(clearButton, &QPushButton::clicked, this, &FindDialog::clear);

    // Layout
    QGridLayout* gFindLayout = new QGridLayout;
    gFindLayout->addWidget(m_nameCheck, 0, 0);
    gFindLayout->addWidget(m_nameLineEdit, 0, 1);
    gFindLayout->addWidget(m_valueCheck, 1, 0);
    gFindLayout->addWidget(m_valueLineEdit, 1, 1);
    gFindLayout->addWidget(m_typeCheck, 2, 0);
    gFindLayout->addWidget(m_typeCombo, 2, 1);

    QVBoxLayout* directionLayout = new QVBoxLayout;
    directionLayout->addWidget(m_forwardRadio);
    directionLayout->addWidget(m_backwardRadio);
    QGroupBox* directionGroup = new QGroupBox("Direction");
    directionGroup->setLayout(directionLayout);

    QVBoxLayout* matchLayout = new QVBoxLayout;
    matchLayout->addWidget(m_matchExactlyRadio);
    matchLayout->addWidget(m_matchContainsRadio);
    matchLayout->addWidget(m_matchStartsWithRadio);
    matchLayout->addWidget(m_matchEndsWithRadio);
    QGroupBox* matchGroupGroup = new QGroupBox("Match Options");
    matchGroupGroup->setLayout(matchLayout);

    QGridLayout* optsLayout = new QGridLayout;
    optsLayout->addWidget(m_matchCaseCheck, 0, 0);
    optsLayout->addWidget(m_wrapAroundCheck, 0, 1);
    optsLayout->addWidget(m_subtreeOnlyCheck, 1, 0);
    optsLayout->addWidget(m_fetchMore, 1, 1);
    QGroupBox* searchOptionsGroup = new QGroupBox("Search Options");
    searchOptionsGroup->setLayout(optsLayout);

    QHBoxLayout* hButtonLayout = new QHBoxLayout;
    hButtonLayout->addWidget(clearButton);
    hButtonLayout->addStretch(1);
    hButtonLayout->addWidget(m_searchButton);
    hButtonLayout->addWidget(cancelButton);

    // Assembly
    QGridLayout* gOuterLayout = new QGridLayout;
    gOuterLayout->addLayout(gFindLayout, 0, 0, 1, 2);
    gOuterLayout->addWidget(directionGroup, 1, 0);
    gOuterLayout->addWidget(matchGroupGroup, 1, 1, 2, 1);
    gOuterLayout->addWidget(searchOptionsGroup, 3, 0, 1, 2);
    gOuterLayout->addLayout(hButtonLayout, 4, 0, 1, 2);
    setLayout(gOuterLayout);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
}

void FindDialog::updateSearchButtonVisibility()
{
    m_searchButton->setEnabled(m_nameCheck->isChecked() || m_valueCheck->isChecked()
                               || m_typeCheck->isChecked());
}

void FindDialog::updateFocus()
{
    if(m_searchButton->isEnabled()) {
        m_searchButton->setFocus();
    } else {
        m_nameLineEdit->setFocus();
    }
}

} // namespace nbt
} // namespace minecraft
