#ifndef MINECRAFTNBTVIEWER_UTIL_VALIDATORS_INT8_VALIDATOR_HPP
#define MINECRAFTNBTVIEWER_UTIL_VALIDATORS_INT8_VALIDATOR_HPP

// Qt
#include <QValidator>

// STL
#include <cstdint>

namespace minecraft {
namespace nbt {

class Int8Validator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(int8_t bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    Q_PROPERTY(int8_t top READ top WRITE setTop NOTIFY topChanged)

public:
    explicit Int8Validator(QObject* parent = nullptr);
    Int8Validator(int8_t bottom, int8_t top, QObject* parent = nullptr);
    ~Int8Validator();

    State validate(QString& input, int& pos) const override;
    void fixup(QString& input) const override;

    void setBottom(int8_t bottom);
    void setTop(int8_t top);
    virtual void setRange(int8_t bottom, int8_t top);

    int8_t bottom() const { return m_b; }
    int8_t top() const { return m_t; }

signals:
    void bottomChanged(int8_t bottom);
    void topChanged(int8_t top);

private:
    Q_DISABLE_COPY_MOVE(Int8Validator)

    int8_t m_b;
    int8_t m_t;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_UTIL_VALIDATORS_INT8_VALIDATOR_HPP
