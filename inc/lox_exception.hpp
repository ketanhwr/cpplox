#pragma once

#include <stdexcept>

#include "token.hpp"

struct interpreter_error: public std::runtime_error
{
    std::shared_ptr<Token> token_;

    interpreter_error(std::shared_ptr<Token> token, const std::string& msg)
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

