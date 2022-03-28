#ifndef AWESOMENBTVIEWER_UTIL_TAG_MIME_DATA_HPP
#define AWESOMENBTVIEWER_UTIL_TAG_MIME_DATA_HPP

// AwesomeMC
#include <AwesomeMC/nbt/tags/abstract_tag.hpp>

// Qt
#include <QMetaType>
#include <QMimeData>

// STL
#include <memory>

Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr);
Q_DECLARE_METATYPE(std::shared_ptr<amc::AbstractTag>);

namespace anv
{

class TagMimeData : public QMimeData
{
public:
    static const QString TagMimeType;

public:
    TagMimeData();
    virtual ~TagMimeData();

    virtual QStringList formats() const;
    virtual bool hasFormat(const QString &mimeType) const;
    virtual QVariant retrieveData(const QString &mimeType, QMetaType type) const;
    void setData(const QString &mimeType, std::shared_ptr<amc::AbstractTag> &data);
    std::shared_ptr<amc::AbstractTag> toTagData() const;

private:
    std::shared_ptr<amc::AbstractTag> m_tag;
    QStringList m_formats;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_UTIL_TAG_MIME_DATA_HPP