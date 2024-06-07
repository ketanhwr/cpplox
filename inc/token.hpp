#pragma once

#include <string>
#include <variant>

enum class TokenType
{
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    LESS, LESS_EQUAL,
    GREATER, GREATER_EQUAL,

    IDENTIFIER, STRING, NUMBER,

    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NUL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    END_OF_FILE,
};

struct Token {
    TokenType tokenType_;
    std::variant<std::string, double> value_;
    std::string lexeme_;
    size_t line_;

    template <typename T>
    Token(TokenType tokenType, T value, std::string lexeme, size_t line)
        : tokenType_{tokenType}
        , value_{value}
        , lexeme_{lexeme}
        , line_{line}
    {}

    friend std::ostream& operator<<(std::ostream& out, Token& token);
};

