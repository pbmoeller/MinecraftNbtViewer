#ifndef MINECRAFTNBTVIEWER_WIDGETS_ABOUT_DIALOG_HPP
#define MINECRAFTNBTVIEWER_WIDGETS_ABOUT_DIALOG_HPP

// Qt
#include <QDialog>

namespace minecraft {
namespace nbt {

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    AboutDialog(QWidget* parent = nullptr);
    virtual ~AboutDialog();

private:
    void setupUi();
    QString makeText();

private slots:
    void openBrowserLink(const QString& link);
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_WIDGETS_ABOUT_DIALOG_HPP
