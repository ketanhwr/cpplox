#include "environment.hpp"

#include "lox_exception.hpp"

Environment::Environment(std::shared_ptr<Environment> parent)
    : parent_{parent}
{ }

void Environment::define(const std::string& name, LoxValuePtr value)
{
    values_.insert_or_assign(name, value);
}

LoxValuePtr Environment::get(TokenPtr token)
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

LoxValuePtr Environment::getAt(int distance, const std::string& name)
{
    return ancestor(distance)->values_.find(name)->second;
}

void Environment::assign(TokenPtr token, LoxValuePtr value)
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

void Environment::assignAt(int distance, TokenPtr token, LoxValuePtr value)
{
    ancestor(distance)->values_.insert_or_assign(token->lexeme_, value);
}

std::shared_ptr<Environment> Environment::ancestor(int distance)
{
    auto env = shared_from_this();

    for (int i = 0; i < distance; ++i) {
        env = env->parent_;
    }

    return env;
}

