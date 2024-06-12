#pragma once

#include "callable.hpp"
#include "stmt.hpp"

struct LoxFunction: public LoxCallable
{
    LoxFunction(FunctionStmtPtr declaration);

    int arity() override;
    LoxValuePtr call(Interpreter& interpreter, std::vector<LoxValuePtr>& args) override;
    
    std::ostream& operator<<(std::ostream& o) override;

private:
    FunctionStmtPtr declaration_;
};

