// MinecraftNbtViewer
#include "compression_dialog.hpp"
#include "ui_compression_dialog.h"

namespace minecraft {
namespace nbt {

CompressionDialog::CompressionDialog(const anvil::CompressionType compression,
                                     QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::CompressionDialog)
{
    m_ui->setupUi(this);

    m_ui->compressionCombo->addItem("GZip", QVariant(static_cast<int>(anvil::CompressionType::Gzip)));
    m_ui->compressionCombo->addItem("Zlib", QVariant(static_cast<int>(anvil::CompressionType::Zlib)));
    m_ui->compressionCombo->addItem("Uncompressed", QVariant(static_cast<int>(anvil::CompressionType::Uncompressed)));
    m_ui->compressionCombo->setCurrentIndex(m_ui->compressionCombo->findData(static_cast<int>(compression)));
}

CompressionDialog::~CompressionDialog() = default;

anvil::CompressionType CompressionDialog::selectedCompressionType() const
{
    return static_cast<anvil::CompressionType>(m_ui->compressionCombo->currentData().toInt());
}

} // namespace nbt
} // namespace minecraft
