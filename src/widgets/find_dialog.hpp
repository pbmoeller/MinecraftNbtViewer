#ifndef MINECRAFTNBTVIEWER_WIDGETS_FIND_DIALOG_HPP
#define MINECRAFTNBTVIEWER_WIDGETS_FIND_DIALOG_HPP

#include "models/nbt_tree_search_helper.hpp"

#include <cpp-anvil/nbt/types.hpp>

// Qt
#include <QDialog>

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
    explicit FindDialog(QWidget* parent = nullptr);
    explicit FindDialog(const SearchCriteria& criteria, QWidget* parent = nullptr);
    virtual ~FindDialog();

    SearchCriteria searchCriteria() const;

protected slots:
    virtual void accept() override;
    void anyCheckStateChanged(Qt::CheckState state);
    void updateNameCheck(const QString& text);
    void updateValueCheck(const QString& text);
    void clear();

private:
    void setSearchCriteria(const SearchCriteria& criteria);
    void setupUi();
    void updateSearchButtonVisibility();

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
