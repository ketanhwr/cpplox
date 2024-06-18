#include "lox_class.hpp"

LoxClass::LoxClass(std::string name)
    : name_{name}
{ }

std::ostream& LoxClass::operator<<(std::ostream& o)
{
    o << "class-" << name_;
    return o;
}

