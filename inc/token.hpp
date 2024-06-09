#pragma once

#include <memory>

#include "value.hpp"

enum class TokenType
{
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    LESS, LESS_EQUAL,
    GREATER, GREATER_EQUAL,

    IDENTIFIER, STRING, NUMBER,

    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    END_OF_FILE,
};

struct Token {
    TokenType tokenType_;
    std::shared_ptr<LoxValue> value_;
    std::string lexeme_;
    size_t line_;

    Token(TokenType tokenType, std::shared_ptr<LoxValue> value, std::string lexeme, size_t line);

    friend std::ostream& operator<<(std::ostream& out, Token& token);
};

