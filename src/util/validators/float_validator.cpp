// MinecraftNbtViewer
#include "float_validator.hpp"

// Qt
#include <qnumeric.h>

// STL
#include <climits>

namespace minecraft {
namespace nbt {

template<typename T>
static inline bool convertDoubleTo(double v, T *value, bool allow_precision_upgrade = true)
{
    static_assert(std::numeric_limits<T>::is_integer);

    // The [conv.fpint] (7.10 Floating-integral conversions) section of the C++
    // standard says only exact conversions are guaranteed. Converting
    // integrals to floating-point with loss of precision has implementation-
    // defined behavior whether the next higher or next lower is returned;
    // converting FP to integral is UB if it can't be represented.
    //
    // That means we can't write UINT64_MAX+1. Writing ldexp(1, 64) would be
    // correct, but Clang, ICC and MSVC don't realize that it's a constant and
    // the math call stays in the compiled code.

    double supremum;
    if(std::numeric_limits<T>::is_signed) {
        supremum = -1.0 * std::numeric_limits<T>::min();    // -1 * (-2^63) = 2^63, exact (for T = qint64)
        *value = std::numeric_limits<T>::min();
        if(v < std::numeric_limits<T>::min())
            return false;
    } else {
        using ST = typename std::make_signed<T>::type;
        supremum = -2.0 * std::numeric_limits<ST>::min();   // -2 * (-2^63) = 2^64, exact (for T = quint64)
        v = fabs(v);
    }
    if constexpr(std::is_integral<T>::value && sizeof(T) > 4 && !allow_precision_upgrade) {
        if(v > double(Q_INT64_C(1) << 53) || v < double(-((Q_INT64_C(1) << 53) + 1)))
            return false;
    }

    *value = std::numeric_limits<T>::max();
    if(v >= supremum)
        return false;

    // Now we can convert, these two conversions cannot be UB
    *value = T(v);

    QT_WARNING_PUSH
        QT_WARNING_DISABLE_FLOAT_COMPARE

        return *value == v;

    QT_WARNING_POP
}

static int numDigits(qint32 n)
{
    if(n == 0) {
        return 1;
    }
    return (int)std::log10(float(n)) + 1;
}

static qint32 pow10(int exp)
{
    qint32 result = 1;
    for(int i = 0; i < exp; ++i) {
        result *= 10;
    }
    return result;
}

FloatValidator::FloatValidator(QObject *parent)
    : FloatValidator(std::numeric_limits<float>::min(),
                     std::numeric_limits<float>::max(),
                     1000, parent)
{

}

FloatValidator::FloatValidator(float bottom, float top, int decimals, QObject *parent)
    : QValidator(parent)
    , m_b(bottom)
    , m_t(top)
    , m_decimals(decimals)
    , m_notation(ScientificNotation)
{

}

FloatValidator::~FloatValidator() = default;

QValidator::State FloatValidator::validate(QString &input, int &pos) const
{
    ((void)pos);

    if(input.isEmpty()) {
        return QValidator::Intermediate;
    }

    const bool startsWithMinus(input[0] == '-');
    if(m_b >= 0 && startsWithMinus) {
        return Invalid;
    }

    const bool startsWithPlus(input[0] == '+');
    if(m_t < 0 && startsWithPlus) {
        return Invalid;
    }

    bool ok = false;
    float f = input.toFloat(&ok);
    if(!ok) {
        return QValidator::Intermediate;
    }

    if(f >= m_b && f <= m_t) {
        return QValidator::Acceptable;
    }

    if(m_notation == StandardNotation) {
        float max = qMax(qAbs(m_b), qAbs(m_t));
        qint32 v;
        if(convertDoubleTo(max, &v)) {
            qint32 n = pow10(numDigits(v));
            if(qAbs(f) > (n - std::pow(10, -m_decimals))) {
                return QValidator::Invalid;
            }
        }
    }

    return QValidator::Intermediate;
}

void FloatValidator::setBottom(float bottom)
{
    setRange(bottom, top(), decimals());
}

void FloatValidator::setTop(float top)
{
    setRange(bottom(), top, decimals());
}

void FloatValidator::setDecimals(int decimals)
{
    setRange(bottom(), top(), decimals);
}

void FloatValidator::setNotation(Notation notation)
{
    if(m_notation != notation) {
        m_notation = notation;
        emit notationChanged(m_notation);
        emit changed();
    }
}

void FloatValidator::setRange(float bottom, float top, int decimals)
{
    bool rangeChanged = false;
    if(m_b != bottom) {
        m_b = bottom;
        rangeChanged = true;
        emit bottomChanged(m_b);
    }

    if(m_t != top) {
        m_t = top;
        rangeChanged = true;
        emit topChanged(m_t);
    }

    if(m_decimals != decimals) {
        m_decimals = decimals;
        rangeChanged = true;
        emit decimalsChanged(m_decimals);
    }
    if(rangeChanged) {
        emit changed();
    }
}

} // namespace nbt
} // namespace minecraft
