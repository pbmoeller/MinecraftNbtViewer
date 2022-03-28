// AwesomeNbtViewer
#include "tag_mime_data.hpp"

namespace anv
{

const QString TagMimeData::TagMimeType = "application/x.amc.nbttag";

TagMimeData::TagMimeData()
    : QMimeData()
{
    m_formats << TagMimeType;
}

TagMimeData::~TagMimeData()
{

}

QStringList TagMimeData::formats() const
{
    return m_formats;
}

bool TagMimeData::hasFormat(const QString &mimeType) const
{
    return m_formats.contains(mimeType);
}

QVariant TagMimeData::retrieveData(const QString &mimeType, QMetaType type) const
{
    if(mimeType == TagMimeType) {
        return QVariant::fromValue(m_tag);
    } else {
        return QMimeData::retrieveData(mimeType, type);
    }
}

void TagMimeData::setData(const QString &mimeType, std::shared_ptr<amc::AbstractTag> &data)
{
    if(mimeType == TagMimeType) {
        m_tag = data;
    }
}

std::shared_ptr<amc::AbstractTag> TagMimeData::toTagData() const
{
    return m_tag;
}

} // namespace anv