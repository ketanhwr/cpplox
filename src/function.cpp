#include "function.hpp"

#include "interpreter.hpp"
#include "lox_exception.hpp"

LoxFunction::LoxFunction(FunctionStmtPtr declaration, std::shared_ptr<Environment> closure)
    : declaration_{declaration}
    , closure_{closure}
{ }

int LoxFunction::arity()
{
    return declaration_->params_->size();
}

LoxValuePtr LoxFunction::call(Interpreter& interpreter, std::vector<LoxValuePtr>& args)
{
    auto env = std::make_shared<Environment>(closure_);

    for (int i = 0; i < args.size(); ++i) {
        env->define(declaration_->params_->at(i)->lexeme_, args.at(i));
    }

    try {
        interpreter.executeBlock(declaration_->body_, env);
    } catch (return_value& retVal) {
        return retVal.value_;
    }

    return std::make_shared<LoxNil>();
}
    
std::ostream& LoxFunction::operator<<(std::ostream& o)
{
    o << "<fn " << declaration_->name_->lexeme_ << ">";
    return o;
}

