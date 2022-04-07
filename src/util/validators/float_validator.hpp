#ifndef AWESOMENBTVIEWER_UTIL_VALIDATORS_FLOAT_VALIDATOR_HPP
#define AWESOMENBTVIEWER_UTIL_VALIDATORS_FLOAT_VALIDATOR_HPP

// Qt
#include <QValidator>

namespace anv
{

class FloatValidator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(float bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    Q_PROPERTY(float top READ top WRITE setTop NOTIFY topChanged)

public:
    explicit FloatValidator(QObject *parent = nullptr);
    FloatValidator(float bottom, float top, int decimals, QObject *parent = nullptr);
    ~FloatValidator();

    enum Notation {
        StandardNotation,
        ScientificNotation
    };
    Q_ENUM(Notation)

    State validate(QString &input, int &pos) const override;

    void setBottom(float bottom);
    void setTop(float top);
    void setDecimals(int decimals);
    void setNotation(Notation notation);
    virtual void setRange(float bottom, float top, int decimals = 0);

    float bottom() const {
        return m_b;
    }
    float top() const {
        return m_t;
    }
    int decimals() const {
        return m_decimals;
    }
    Notation notation() const {
        return m_notation;
    }

signals:
    void bottomChanged(float bottom);
    void topChanged(float top);
    void decimalsChanged(int decimals);
    void notationChanged(Notation notation);
    
private:
    Q_DISABLE_COPY_MOVE(FloatValidator);

    float m_b;
    float m_t;
    int m_decimals;
    Notation m_notation;
};

} // namespace anv

#endif // AWESOMENBTVIEWER_UTIL_VALIDATORS_FLOAT_VALIDATOR_HPP