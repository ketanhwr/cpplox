#pragma once

#include <vector>
#include <string>
#include <optional>

#include "token.hpp"

class Scanner
{
    const std::string& program_;
    std::vector<std::shared_ptr<Token>> tokens_;

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

    void addToken(TokenType tokenType, std::shared_ptr<LoxValue> value)
    {
        std::string lexeme = program_.substr(start_, current_ - start_);
        tokens_.push_back(std::make_shared<Token>(tokenType, value, lexeme, line_));
    }

    void parseString();
    void parseNumber();
    void parseIdentifier();

    bool isNum(char c);
    bool isAlpha(char c);
    bool isAlphaNum(char c);

public:
    Scanner(const std::string& program);

    std::optional<std::vector<std::shared_ptr<Token>>> scanTokens();
};

