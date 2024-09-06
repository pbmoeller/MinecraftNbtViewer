#ifndef MINECRAFTNBTVIEWER_COMPRESSION_DIALOG_HPP
#define MINECRAFTNBTVIEWER_COMPRESSION_DIALOG_HPP

// cpp-anvil
#include <cpp-anvil/util/compression.hpp>

// Qt
#include <QDialog>

namespace Ui {
class CompressionDialog;
}

namespace minecraft {
namespace nbt {

class CompressionDialog : public QDialog
{
    Q_OBJECT

public:
    CompressionDialog(const anvil::CompressionType compression,
                      QWidget *parent = nullptr);
    virtual ~CompressionDialog();

    anvil::CompressionType selectedCompressionType() const;

private:
    Ui::CompressionDialog *m_ui;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_COMPRESSION_DIALOG_HPP
