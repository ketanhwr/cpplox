#pragma once

#include <vector>
#include <string>
#include <optional>

#include "token.hpp"

class Scanner
{
    const std::string& program_;
    std::vector<Token> tokens_;

    bool hasError_{false};

    size_t current_{0};
    size_t start_{0};

    size_t line_{1};

    bool atEnd();

    char advance();
    char peek();
    char peekNext();

    bool match(char c);
    
    void addToken(TokenType tokenType);

    template <typename T>
    void addToken(TokenType tokenType, T value)
    {
        std::string lexeme = program_.substr(start_, current_ - start_);
        tokens_.emplace_back(tokenType, value, lexeme, line_);
    }

    void parseString();
    void parseNumber();
    void parseIdentifier();

    bool isNum(char c);
    bool isAlpha(char c);
    bool isAlphaNum(char c);

public:
    Scanner(const std::string& program);

    std::optional<std::vector<Token>> scanTokens();
};

