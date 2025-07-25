// MinecraftNbtViewer
#include "int32_validator.hpp"

// STL
#include <climits>

namespace minecraft {
namespace nbt {

Int32Validator::Int32Validator(QObject* parent)
    : Int32Validator(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max(),
                     parent)
{ }

Int32Validator::Int32Validator(int32_t bottom, int32_t top, QObject* parent)
    : QValidator(parent)
    , m_b(bottom)
    , m_t(top)
{ }

Int32Validator::~Int32Validator() = default;

QValidator::State Int32Validator::validate(QString& input, int& pos) const
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

void Int32Validator::fixup(QString& input) const
{
    Q_UNUSED(input);
}

void Int32Validator::setBottom(int32_t bottom)
{
    setRange(bottom, top());
}

void Int32Validator::setTop(int32_t top)
{
    setRange(bottom(), top);
}

void Int32Validator::setRange(int32_t bottom, int32_t top)
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
