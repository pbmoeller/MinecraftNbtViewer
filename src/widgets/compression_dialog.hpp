#ifndef MINECRAFTNBTVIEWER_COMPRESSION_DIALOG_HPP
#define MINECRAFTNBTVIEWER_COMPRESSION_DIALOG_HPP

// cpp-anvil
#include <cpp-anvil/util/compression.hpp>

// Qt
#include <QDialog>

namespace Ui {
class CompressionDialog;
}

namespace anv {

class CompressionDialog : public QDialog
{
    Q_OBJECT

public:
    CompressionDialog(const anvil::CompressionType compression,
                      QWidget *parent = nullptr);
    virtual ~CompressionDialog();

    anvil::CompressionType getSelectedCompressionType() const;

private:
    Ui::CompressionDialog *m_ui;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_COMPRESSION_DIALOG_HPP
