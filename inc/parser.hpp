#pragma once

#include "expr.hpp"
#include "token.hpp"

#include <vector>

class Parser
{
    struct parsing_error: public std::runtime_error
    {
        parsing_error(const std::string& what)
            : std::runtime_error{what}
        {}
    };

    const std::vector<std::shared_ptr<Token>>& tokens_;
    size_t current_{0};

    parsing_error error(std::shared_ptr<Token> token, const std::string& msg);

    // Helpers
    std::shared_ptr<Token> peek();
    std::shared_ptr<Token> previous();
    std::shared_ptr<Token> advance();
    bool atEnd();
    bool check(TokenType tokenType);
    std::shared_ptr<Token> consume(TokenType tokenType, const std::string& msg);

    bool match(TokenType tokenType)
    {
        if (check(tokenType)) {
            advance();
            return true;
        }
        return false;
    }

    template <typename... Args>
    bool match(TokenType tokenType, Args... args)
    {
        return match(tokenType) || match(args...);
    }

    // Parsers
    std::shared_ptr<Expr> parseExpression();
    std::shared_ptr<Expr> parseEquality();
    std::shared_ptr<Expr> parseComparison();
    std::shared_ptr<Expr> parseTerm();
    std::shared_ptr<Expr> parseFactor();
    std::shared_ptr<Expr> parseUnary();
    std::shared_ptr<Expr> parsePrimary();
public:
    Parser(const std::vector<std::shared_ptr<Token>>& tokens);

    std::shared_ptr<Expr> parse();
};

