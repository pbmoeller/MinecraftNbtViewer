// AwesomeNbtViewer
#include "int64_validator.hpp"

// STL
#include <climits>

namespace anv
{

Int64Validator::Int64Validator(QObject *parent)
    : Int64Validator(std::numeric_limits<int64_t>::min(),
                     std::numeric_limits<int64_t>::max(),
                     parent)
{

}

Int64Validator::Int64Validator(int64_t bottom, int64_t top, QObject *parent)
    : QValidator(parent)
    , m_b(bottom)
    , m_t(top)
{

}

Int64Validator::~Int64Validator()
{

}

QValidator::State Int64Validator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);

    if(input.isEmpty()) {
        return Acceptable;
    }

    const bool startsWithMinus(input[0] == '-');
    if(m_b >= 0 && startsWithMinus) {
        return Invalid;
    }

    const bool startsWithPlus(input[0] == '+');
    if(m_t < 0 && startsWithPlus) {
        return Invalid;
    }

    if(input.size() == 1 && (startsWithPlus || startsWithMinus)) {
        return Intermediate;
    }

    bool ok;
    qlonglong entered = input.toLongLong(&ok, 10);
    if(!ok) {
        return Invalid;
    }

    if(entered >= m_b && entered <= m_t) {
        return ok ? Acceptable : Intermediate;
    } else {
        return Invalid;
    }
}

void Int64Validator::fixup(QString &input) const
{
    Q_UNUSED(input);
}

void Int64Validator::setBottom(int64_t bottom)
{
    setRange(bottom, top());
}

void Int64Validator::setTop(int64_t top)
{
    setRange(bottom(), top);
}

void Int64Validator::setRange(int64_t bottom, int64_t top)
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

    if(rangeChanged) {
        emit changed();
    }
}

} // namespace anv