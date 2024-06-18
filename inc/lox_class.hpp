#pragma once

#include "value.hpp"

struct LoxClass: public LoxValue
{
    std::string name_;

    LoxClass(std::string name);
    ~LoxClass() = default;

    std::ostream& operator<<(std::ostream& o) override;
};

