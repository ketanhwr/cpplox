#pragma once

#include <unordered_map>
#include <string>

#include "value.hpp"
#include "token.hpp"

struct Environment
{
    Environment() = default;
    Environment(std::shared_ptr<Environment> parent);

    void define(const std::string& name, LoxValuePtr value);

    LoxValuePtr get(std::shared_ptr<Token> token);

    void assign(std::shared_ptr<Token> token, LoxValuePtr value);

private:
    std::unordered_map<std::string, LoxValuePtr> values_;
    std::shared_ptr<Environment> parent_;

};
