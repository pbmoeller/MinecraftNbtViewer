// MinecraftNbtViewer
#include "int8_validator.hpp"

// STL
#include <climits>

namespace minecraft {
namespace nbt {

Int8Validator::Int8Validator(QObject *parent)
    : Int8Validator(std::numeric_limits<int8_t>::min(),
                    std::numeric_limits<int8_t>::max(),
                    parent)
{

}

Int8Validator::Int8Validator(int8_t bottom, int8_t top, QObject *parent)
    : QValidator(parent)
    , m_b(bottom)
    , m_t(top)
{

}

Int8Validator::~Int8Validator()
{

}

QValidator::State Int8Validator::validate(QString &input, int &pos) const
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

void Int8Validator::fixup(QString &input) const
{
    Q_UNUSED(input);
}

void Int8Validator::setBottom(int8_t bottom)
{
    setRange(bottom, top());
}

void Int8Validator::setTop(int8_t top)
{
    setRange(bottom(), top);
}

void Int8Validator::setRange(int8_t bottom, int8_t top)
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

} // namespace nbt
} // namespace minecraft
