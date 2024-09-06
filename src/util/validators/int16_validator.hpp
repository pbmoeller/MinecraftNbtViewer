#ifndef MINECRAFTNBTVIEWER_UTIL_VALIDATORS_INT16_VALIDATOR_HPP
#define MINECRAFTNBTVIEWER_UTIL_VALIDATORS_INT16_VALIDATOR_HPP

// Qt
#include <QValidator>

// STL
#include <cstdint>

namespace minecraft {
namespace nbt {

class Int16Validator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(int16_t bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    Q_PROPERTY(int16_t top READ top WRITE setTop NOTIFY topChanged)

public:
    explicit Int16Validator(QObject *parent = nullptr);
    Int16Validator(int16_t bottom, int16_t top, QObject *parent = nullptr);
    ~Int16Validator();

    State validate(QString &input, int &pos) const override;
    void fixup(QString &input) const override;

    void setBottom(int16_t bottom);
    void setTop(int16_t top);
    virtual void setRange(int16_t bottom, int16_t top);

    int16_t bottom() const {
        return m_b;
    }
    int16_t top() const {
        return m_t;
    }

signals:
    void bottomChanged(int16_t bottom);
    void topChanged(int16_t top);

private:
    Q_DISABLE_COPY_MOVE(Int16Validator);

    int16_t m_b;
    int16_t m_t;
};

} // namespace nbt
} // namespace minecraft

#endif // MINECRAFTNBTVIEWER_UTIL_VALIDATORS_INT16_VALIDATOR_HPP
