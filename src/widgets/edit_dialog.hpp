#ifndef MINECRAFTNBTVIEWER_WIDGETS_EDIT_DIALOG_HPP
#define MINECRAFTNBTVIEWER_WIDGETS_EDIT_DIALOG_HPP

// cpp-anvil
#include <cpp-anvil/nbt/basic_tag.hpp>

// Qt
#include <QDialog>
#include <QString>

class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;

namespace anv {

// Forward declarations
class NbtTreeItemNbtTag;
class NbtDataTreeModel;

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    enum class EditFunction
    {
        Rename,
        EditValue,
    };

public:
    EditDialog(NbtTreeItemNbtTag *treeItem,
               NbtDataTreeModel *model,
               EditFunction function = EditFunction::Rename,
               QWidget *parent = nullptr);
    virtual ~EditDialog();

protected slots:
    virtual void accept() override;

private:
    void setupUi(EditFunction function);
    QString valueToString() const;
    QString arrayToString() const;
    bool checkAndSetValue(const QString &value, anvil::BasicTag *tag);
    bool checkAndSetArrayValue(const QString &value, anvil::BasicTag *tag);

private:
    // Data
    NbtTreeItemNbtTag *m_treeItem;
    NbtDataTreeModel *m_model;
    bool m_hasRenameField;
    bool m_hasEditField;

    // Ui elements
    QLineEdit *m_lineEditName;
    QLineEdit *m_lineEditValue;
    QTextEdit *m_textEdit;
};

} // namespace anv

#endif // MINECRAFTNBTVIEWER_WIDGETS_EDIT_DIALOG_HPP
