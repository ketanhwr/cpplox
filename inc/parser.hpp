#pragma once

#include "expr.hpp"
#include "token.hpp"
#include "stmt.hpp"
#include "lox_exception.hpp"

#include <optional>
#include <vector>

class Parser
{
    const std::vector<std::shared_ptr<Token>>& tokens_;
    size_t current_{0};
    bool parsing_failed_{false};

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

    std::shared_ptr<Stmt> parsePrintStmt();
    std::shared_ptr<Stmt> parseExpressionStmt();
    std::shared_ptr<Stmt> parseStatement();
    std::shared_ptr<Stmt> parseVarDeclaration();
    std::shared_ptr<Stmt> parseDeclaration();

    void synchronize();
public:
    Parser(const std::vector<std::shared_ptr<Token>>& tokens);

    std::optional<std::vector<std::shared_ptr<Stmt>>> parse();
};

