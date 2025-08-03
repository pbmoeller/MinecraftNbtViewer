#ifndef MINECRAFTNBTVIEWER_WIDGETS_FIND_DIALOG_HPP
#define MINECRAFTNBTVIEWER_WIDGETS_FIND_DIALOG_HPP

#include "models/nbt_tree_search_helper.hpp"

#include <cpp-anvil/nbt/types.hpp>

// Qt
#include <QDialog>

#include <optional>

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;

namespace minecraft {
namespace nbt {

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget* parent = nullptr);
    virtual ~FindDialog();

    SearchCriteria searchCriteria() const;

protected slots:
    virtual void accept() override;
    void updateSearchButtonVisibility(bool checked = false);
    void updateNameCheck(const QString& text);
    void updateValueCheck(const QString& text);

private:
    void setupUi();

private:
    QCheckBox* m_nameCheck;
    QCheckBox* m_valueCheck;
    QCheckBox* m_typeCheck;
    QCheckBox* m_directionCheck;
    QCheckBox* m_matchCaseCheck;
    QCheckBox* m_wrapAroundCheck;
    QCheckBox* m_downwardsOnlyCheck;
    QLineEdit* m_nameLineEdit;
    QLineEdit* m_valueLineEdit;
    QComboBox* m_typeCombo;
    QPushButton* m_searchButton;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_WIDGETS_FIND_DIALOG_HPP
