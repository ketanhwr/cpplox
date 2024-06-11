#include "environment.hpp"

#include "lox_exception.hpp"

void Environment::define(const std::string& name, LoxValuePtr value)
{
    values.insert({name, value});
}

LoxValuePtr Environment::get(std::shared_ptr<Token> token)
{
    auto it = values.find(token->lexeme_);

    if (it != values.end()) {
        return it->second;
    }

    std::string errorMsg_{"Undefined variable '"};
    errorMsg_.append(token->lexeme_);
    errorMsg_.append(1, '\'');

    throw interpreter_error{token, std::move(errorMsg_)};
}

