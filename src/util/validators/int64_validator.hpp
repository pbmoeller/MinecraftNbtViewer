#ifndef AWESOMENBTVIEWER_UTIL_VALIDATORS_INT64_VALIDATOR_HPP
#define AWESOMENBTVIEWER_UTIL_VALIDATORS_INT64_VALIDATOR_HPP

// Qt
#include <QValidator>

// STL
#include <cstdint>

namespace anv
{

class Int64Validator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(int64_t bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    Q_PROPERTY(int64_t top READ top WRITE setTop NOTIFY topChanged)

public:
    explicit Int64Validator(QObject *parent = nullptr);
    Int64Validator(int64_t bottom, int64_t top, QObject *parent = nullptr);
    ~Int64Validator();

    State validate(QString &input, int &pos) const override;
    void fixup(QString &input) const override;

    void setBottom(int64_t bottom);
    void setTop(int64_t top);
    virtual void setRange(int64_t bottom, int64_t top);

    int64_t bottom() const {
        return m_b;
    }
    int64_t top() const {
        return m_t;
    }

signals:
    void bottomChanged(int64_t bottom);
    void topChanged(int64_t top);

private:
    Q_DISABLE_COPY_MOVE(Int64Validator);

    int64_t m_b;
    int64_t m_t;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_UTIL_VALIDATORS_INT64_VALIDATOR_HPP