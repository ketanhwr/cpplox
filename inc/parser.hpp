#pragma once

#include "expr.hpp"
#include "token.hpp"
#include "stmt.hpp"
#include "lox_exception.hpp"

#include <optional>
#include <vector>

class Parser
{
    const std::vector<TokenPtr>& tokens_;
    size_t current_{0};
    bool parsing_failed_{false};

    parsing_error error(TokenPtr token, const std::string& msg);

    // Helpers
    TokenPtr peek();
    TokenPtr previous();
    TokenPtr advance();
    bool atEnd();
    bool check(TokenType tokenType);
    TokenPtr consume(TokenType tokenType, const std::string& msg);

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
    ExprPtr parseExpression();
    ExprPtr parseAssignment();
    ExprPtr parseEquality();
    ExprPtr parseComparison();
    ExprPtr parseTerm();
    ExprPtr parseFactor();
    ExprPtr parseUnary();
    ExprPtr parseCall();
    ExprPtr parsePrimary();
    ExprPtr parseOr();
    ExprPtr parseAnd();
    ExprPtr finishCall(ExprPtr expr);

    StmtPtr parseForStmt();
    StmtPtr parseWhileStmt();
    StmtPtr parseIfStmt();
    decltype(BlockStmt::statements_) parseBlock();
    StmtPtr parsePrintStmt();
    StmtPtr parseReturnStmt();
    StmtPtr parseExpressionStmt();
    StmtPtr parseStatement();
    StmtPtr parseVarDeclaration();
    FunctionStmtPtr parseFunction(const std::string& kind);
    StmtPtr parseClass();
    StmtPtr parseDeclaration();

    void synchronize();
public:
    Parser(const std::vector<TokenPtr>& tokens);

    std::optional<std::vector<StmtPtr>> parse();
};

