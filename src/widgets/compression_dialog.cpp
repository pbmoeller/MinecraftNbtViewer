// AwesomeNbtViewer
#include "compression_dialog.hpp"
#include "ui_compression_dialog.h"

namespace anv
{

CompressionDialog::CompressionDialog(const amc::CompressionType compression,
                                     QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::CompressionDialog)
{
    m_ui->setupUi(this);

    m_ui->compressionCombo->addItem("GZip", QVariant(static_cast<int>(amc::CompressionType::GZip)));
    m_ui->compressionCombo->addItem("Zlib", QVariant(static_cast<int>(amc::CompressionType::Zlib)));
    m_ui->compressionCombo->addItem("Uncompressed", QVariant(static_cast<int>(amc::CompressionType::Uncompressed)));
    m_ui->compressionCombo->setCurrentIndex(m_ui->compressionCombo->findData(static_cast<int>(compression)));
}

CompressionDialog::~CompressionDialog()
{

}

amc::CompressionType CompressionDialog::getSelectedCompressionType() const
{
    return static_cast<amc::CompressionType>(m_ui->compressionCombo->currentData().toInt());
}

} // namespace anv