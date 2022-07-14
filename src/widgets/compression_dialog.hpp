#ifndef AWESOMENBTVIEWER_COMPRESSION_DIALOG_HPP
#define AWESOMENBTVIEWER_COMPRESSION_DIALOG_HPP

// AwesomeMC
#include <AwesomeMC/util/compression.hpp>

// Qt
#include <QDialog>

namespace Ui
{
class CompressionDialog;
}

namespace anv
{

class CompressionDialog : public QDialog
{
    Q_OBJECT

public:
    CompressionDialog(const amc::CompressionType compression,
                      QWidget *parent = nullptr);
    virtual ~CompressionDialog();

    amc::CompressionType getSelectedCompressionType() const;

private:
    Ui::CompressionDialog *m_ui;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_COMPRESSION_DIALOG_HPP