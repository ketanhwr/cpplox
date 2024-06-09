#include "value.hpp"

#include <ostream>

std::ostream& operator<<(std::ostream& o, LoxValue& value)
{
    value.operator<<(o);
    return o;
}

LoxString::LoxString(std::string value)
    : value_{value}
{}

std::ostream& LoxString::operator<<(std::ostream& o)
{
    o << value_;
    return o;
}

LoxInteger::LoxInteger(int64_t value)
    : value_{value}
{}

std::ostream& LoxInteger::operator<<(std::ostream& o)
{
    o << value_;
    return o;
}

LoxFloat::LoxFloat(double value)
    : value_{value}
{}

std::ostream& LoxFloat::operator<<(std::ostream& o)
{
    o << value_;
    return o;
}

LoxBool::LoxBool(bool value)
    : value_{value}
{}

std::ostream& LoxBool::operator<<(std::ostream& o)
{
    o << value_;
    return o;
}

std::ostream& LoxNil::operator<<(std::ostream& o)
{
    o << "nil";
    return o;
}

