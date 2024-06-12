#pragma once

#include "callable.hpp"

struct NativeClock: public LoxCallable
{
    int arity() override;
    LoxValuePtr call(Interpreter& interpreter, std::vector<LoxValuePtr>& args) override;
    
    std::ostream& operator<<(std::ostream& o) override;
};

