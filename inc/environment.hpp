#pragma once

#include <unordered_map>
#include <string>

#include "value.hpp"
#include "token.hpp"

struct Environment
{
    std::unordered_map<std::string, LoxValuePtr> values;

    void define(const std::string& name, LoxValuePtr value);

    LoxValuePtr get(std::shared_ptr<Token> token);
};
