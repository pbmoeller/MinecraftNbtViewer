// MinecraftNbtViewer
#include "int16_validator.hpp"

// STL
#include <climits>

namespace minecraft {
namespace nbt {

Int16Validator::Int16Validator(QObject* parent)
    : Int16Validator(std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max(),
                     parent)
{ }

Int16Validator::Int16Validator(int16_t bottom, int16_t top, QObject* parent)
    : QValidator(parent)
    , m_b(bottom)
    , m_t(top)
{ }

Int16Validator::~Int16Validator() = default;

QValidator::State Int16Validator::validate(QString& input, int& pos) const
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

void Int16Validator::fixup(QString& input) const
{
    Q_UNUSED(input);
}

void Int16Validator::setBottom(int16_t bottom)
{
    setRange(bottom, top());
}

void Int16Validator::setTop(int16_t top)
{
    setRange(bottom(), top);
}

void Int16Validator::setRange(int16_t bottom, int16_t top)
{
    bool rangeChanged = false;
    if(m_b != bottom) {
        m_b          = bottom;
        rangeChanged = true;
        emit bottomChanged(m_b);
    }

    if(m_t != top) {
        m_t          = top;
        rangeChanged = true;
        emit topChanged(m_t);
    }

    if(rangeChanged) {
        emit changed();
    }
}

} // namespace nbt
} // namespace minecraft
