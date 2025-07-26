#ifndef MINECRAFTNBTVIEWER_UTIL_VALIDATORS_INT32_VALIDATOR_HPP
#define MINECRAFTNBTVIEWER_UTIL_VALIDATORS_INT32_VALIDATOR_HPP

// Qt
#include <QValidator>

// STL
#include <cstdint>

namespace minecraft {
namespace nbt {

class Int32Validator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(int32_t bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    Q_PROPERTY(int32_t top READ top WRITE setTop NOTIFY topChanged)

public:
    explicit Int32Validator(QObject* parent = nullptr);
    Int32Validator(int32_t bottom, int32_t top, QObject* parent = nullptr);
    ~Int32Validator();

    State validate(QString& input, int& pos) const override;
    void fixup(QString& input) const override;

    void setBottom(int32_t bottom);
    void setTop(int32_t top);
    virtual void setRange(int32_t bottom, int32_t top);

    int32_t bottom() const { return m_b; }
    int32_t top() const { return m_t; }

signals:
    void bottomChanged(int32_t bottom);
    void topChanged(int32_t top);

private:
    Q_DISABLE_COPY_MOVE(Int32Validator)

    int32_t m_b;
    int32_t m_t;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_UTIL_VALIDATORS_INT32_VALIDATOR_HPP
