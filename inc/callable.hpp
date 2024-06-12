#pragma once

#include <vector>

#include "value.hpp"

struct Interpreter;

struct LoxCallable: public LoxValue
{
    virtual int arity() = 0;
    virtual LoxValuePtr call(Interpreter& interpreter, std::vector<LoxValuePtr>& args) = 0;
};

