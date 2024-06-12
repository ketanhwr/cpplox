#pragma once

#include "callable.hpp"
#include "stmt.hpp"

struct Environment;

struct LoxFunction: public LoxCallable
{
    LoxFunction(FunctionStmtPtr declaration, std::shared_ptr<Environment> closure);

    int arity() override;
    LoxValuePtr call(Interpreter& interpreter, std::vector<LoxValuePtr>& args) override;
    
    std::ostream& operator<<(std::ostream& o) override;

private:
    FunctionStmtPtr declaration_;
    std::shared_ptr<Environment> closure_;
};

