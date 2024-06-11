#include "environment.hpp"

#include "lox_exception.hpp"

Environment::Environment(std::shared_ptr<Environment> parent)
    : parent_{parent}
{ }

void Environment::define(const std::string& name, LoxValuePtr value)
{
    values_.insert({name, value});
}

LoxValuePtr Environment::get(std::shared_ptr<Token> token)
{
    auto it = values_.find(token->lexeme_);

    if (it != values_.end()) {
        return it->second;
    }

    if (parent_) {
        return parent_->get(token);
    }

    std::string errorMsg_{"Undefined variable '"};
    errorMsg_.append(token->lexeme_);
    errorMsg_.append(1, '\'');

    throw interpreter_error{token, std::move(errorMsg_)};
}

void Environment::assign(std::shared_ptr<Token> token, LoxValuePtr value)
{
    auto it = values_.find(token->lexeme_);

    if (it != values_.end()) {
        it->second = value;
        return;
    }

    if (parent_) {
        parent_->assign(token, value);
        return;
    }

    std::string errorMsg_{"Undefined variable '"};
    errorMsg_.append(token->lexeme_);
    errorMsg_.append(1, '\'');

    throw interpreter_error{token, std::move(errorMsg_)};
}

