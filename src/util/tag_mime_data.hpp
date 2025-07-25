#ifndef MINECRAFTNBTVIEWER_UTIL_TAG_MIME_DATA_HPP
#define MINECRAFTNBTVIEWER_UTIL_TAG_MIME_DATA_HPP

// cpp-anvil
#include <cpp-anvil/nbt/basic_tag.hpp>

// Qt
#include <QMetaType>
#include <QMimeData>

// STL
#include <memory>

Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr);
Q_DECLARE_METATYPE(std::shared_ptr<anvil::BasicTag>);

namespace minecraft {
namespace nbt {

class TagMimeData : public QMimeData
{
public:
    static const QString TagMimeType;

public:
    TagMimeData();
    virtual ~TagMimeData();

    virtual QStringList formats() const;
    virtual bool hasFormat(const QString& mimeType) const;
    virtual QVariant retrieveData(const QString& mimeType, QMetaType type) const;
    void setData(const QString& mimeType, std::shared_ptr<anvil::BasicTag>& data);
    std::shared_ptr<anvil::BasicTag> toTagData() const;

private:
    std::shared_ptr<anvil::BasicTag> m_tag;
    QStringList m_formats;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_UTIL_TAG_MIME_DATA_HPP
