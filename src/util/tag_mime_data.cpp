// MinecraftNbtViewer
#include "tag_mime_data.hpp"

namespace minecraft {
namespace nbt {

const QString TagMimeData::TagMimeType = "application/x.cpp-anvil.nbttag";

TagMimeData::TagMimeData()
    : QMimeData()
{
    m_formats << TagMimeType;
}

TagMimeData::~TagMimeData() = default;

QStringList TagMimeData::formats() const
{
    return m_formats;
}

bool TagMimeData::hasFormat(const QString& mimeType) const
{
    return m_formats.contains(mimeType);
}

QVariant TagMimeData::retrieveData(const QString& mimeType, QMetaType type) const
{
    if(mimeType == TagMimeType) {
        return QVariant::fromValue(m_tag);
    } else {
        return QMimeData::retrieveData(mimeType, type);
    }
}

void TagMimeData::setData(const QString& mimeType, std::shared_ptr<anvil::BasicTag>& data)
{
    if(mimeType == TagMimeType) {
        m_tag = data;
    }
}

std::shared_ptr<anvil::BasicTag> TagMimeData::toTagData() const
{
    return m_tag;
}

} // namespace nbt
} // namespace minecraft
