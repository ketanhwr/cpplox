#pragma once

#include <stdexcept>

#include "token.hpp"

struct interpreter_error: public std::runtime_error
{
    TokenPtr token_;

    interpreter_error(TokenPtr token, const std::string& msg)
        : token_{token}
        , std::runtime_error{msg}
    {}
};

struct parsing_error: public std::runtime_error
{
    parsing_error(const std::string& what)
        : std::runtime_error{what}
    {}
};

struct return_value: public std::runtime_error
{
    LoxValuePtr value_;

    return_value(LoxValuePtr value)
        : value_{value}
        , std::runtime_error{""}
    {}
};

