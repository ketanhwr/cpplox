#include "parser.hpp"

#include <iostream>

Parser::Parser(const std::vector<TokenPtr>& tokens)
    : tokens_{tokens}
{}

auto Parser::error(TokenPtr token, const std::string& msg) -> parsing_error
{
    if (token->tokenType_ == TokenType::END_OF_FILE) {
        std::cerr << "Line [" << token->line_ << "] Error at end: " << msg << std::endl;
    } else {
        std::cerr << "Line [" << token->line_ << "] Error at " << token->lexeme_ << ": " << msg << std::endl;
    }

    return parsing_error{""};
}

TokenPtr Parser::peek()
{
    return tokens_[current_];
}

TokenPtr Parser::previous()
{
    return tokens_[current_ - 1];
}

TokenPtr Parser::advance()
{
    if (!atEnd()) {
        ++current_;
    }
    return previous();
}

bool Parser::atEnd()
{
    return peek()->tokenType_ == TokenType::END_OF_FILE;
}

bool Parser::check(TokenType tokenType)
{
    if (atEnd()) return false;
    return peek()->tokenType_ == tokenType;
}

TokenPtr Parser::consume(TokenType tokenType, const std::string& msg)
{
    if (check(tokenType)) return advance();

    throw error(peek(), msg);
}

ExprPtr Parser::parseExpression()
{
    return parseAssignment();
}

ExprPtr Parser::parseAssignment()
{
    auto expr = parseOr();

    if (match(TokenType::EQUAL)) {
        auto equals = previous();
        auto value = parseAssignment();

        if (auto var = std::dynamic_pointer_cast<VariableExpr>(expr)) {
            return std::make_shared<AssignExpr>(var->name_, value);
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}

ExprPtr Parser::parseOr()
{
    auto expr = parseAnd();

    while (match(TokenType::OR)) {
        auto op = previous();
        auto right = parseAnd();

        expr = std::make_shared<LogicalExpr>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::parseAnd()
{
    auto expr = parseEquality();

    while (match(TokenType::AND)) {
        auto op = previous();
        auto right = parseEquality();

        expr = std::make_shared<LogicalExpr>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::parseEquality()
{
    auto expr = parseComparison();

    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        auto op = previous();
        auto right = parseComparison();

        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::parseComparison()
{
    auto expr = parseTerm();

    while (match(TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREATER, TokenType::GREATER_EQUAL)) {
        auto op = previous();
        auto right = parseTerm();

        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::parseTerm()
{
    auto expr = parseFactor();

    while (match(TokenType::MINUS, TokenType::PLUS)) {
        auto op = previous();
        auto right = parseFactor();

        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::parseFactor()
{
    auto expr = parseUnary();

    while (match(TokenType::STAR, TokenType::SLASH)) {
        auto op = previous();
        auto right = parseUnary();

        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::parseUnary()
{
    if (match(TokenType::BANG, TokenType::MINUS)) {
        auto op = previous();
        auto right = parseUnary();

        return std::make_shared<UnaryExpr>(op, right);
    }

    return parseCall();
}

ExprPtr Parser::parseCall()
{
    auto expr = parsePrimary();

    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            expr = finishCall(expr);
        } else {
            break;
        }
    }

    return expr;
}

ExprPtr Parser::finishCall(ExprPtr expr)
{
    auto args = std::make_shared<std::vector<ExprPtr>>();

    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (args->size() >= 255) {
                error(peek(), "Can't have more than 255 argumnets");
            }
            args->push_back(parseExpression());
        } while (match(TokenType::COMMA));
    }

    auto paren = consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments");

    return std::make_shared<CallExpr>(expr, paren, args);
}

ExprPtr Parser::parsePrimary()
{
    if (match(TokenType::TRUE, TokenType::FALSE)) {
        return std::make_shared<LiteralExpr>(std::make_shared<LoxBool>(previous()->tokenType_ == TokenType::TRUE));
    }

    if (match(TokenType::NIL)) {
        return std::make_shared<LiteralExpr>(std::make_shared<LoxNil>());
    }

    if (match(TokenType::NUMBER, TokenType::STRING)) {
        return std::make_shared<LiteralExpr>(previous()->value_);
    }

    if (match(TokenType::LEFT_PAREN)) {
        auto exp = parseExpression();

        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");

        return std::make_shared<GroupingExpr>(exp);
    }

    if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<VariableExpr>(previous());
    }

    throw error(peek(), "Expected expression");
}

StmtPtr Parser::parseForStmt()
{
    consume(TokenType::LEFT_PAREN, "Expected '(' after for");

    StmtPtr init;
    if (match(TokenType::SEMICOLON)) {
        init = nullptr;
    } else if (match(TokenType::VAR)) {
        init = parseVarDeclaration();
    } else {
        init = parseExpressionStmt();
    }

    ExprPtr cond;
    if (!check(TokenType::SEMICOLON)) {
        cond = parseExpression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after for-condition");

    ExprPtr increment;
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = parseExpression();
    }

    consume(TokenType::RIGHT_PAREN, "Expected ')' after for clauses");

    StmtPtr body = parseStatement();

    if (increment) {
        body = std::make_shared<BlockStmt>(
                std::make_shared<std::vector<StmtPtr>>(
                    std::initializer_list<StmtPtr>{body, std::make_shared<ExpressionStmt>(increment)}
                )
            );
    }

    if (!cond) {
        cond = std::make_shared<LiteralExpr>(std::make_shared<LoxBool>(true));
    }

    body = std::make_shared<WhileStmt>(cond, body);

    if (init) {
        body = std::make_shared<BlockStmt>(
                std::make_shared<std::vector<StmtPtr>>(
                    std::initializer_list<StmtPtr>{init, body}
                )
            );
    }
    
    return body;
}

StmtPtr Parser::parseWhileStmt()
{
    consume(TokenType::LEFT_PAREN, "Expected '(' after while");

    auto cond = parseExpression();

    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition");

    auto whileBlock = parseStatement();

    return std::make_shared<WhileStmt>(cond, whileBlock);
}

StmtPtr Parser::parseIfStmt()
{
    consume(TokenType::LEFT_PAREN, "Expected '(' after if");

    auto cond = parseExpression();
    
    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition");

    auto thenBlock = parseStatement();

    StmtPtr elseBlock;
    if (match(TokenType::ELSE)) {
        elseBlock = parseStatement();
    }

    return std::make_shared<IfStmt>(cond, thenBlock, elseBlock);
}

decltype(BlockStmt::statements_) Parser::parseBlock()
{
    auto statements = std::make_shared<std::vector<StmtPtr>>();

    while (!check(TokenType::RIGHT_BRACE) && !atEnd()) {
        statements->push_back(parseDeclaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expected '}' after block");

    return statements;
}

StmtPtr Parser::parsePrintStmt()
{
    auto exp = parseExpression();

    consume(TokenType::SEMICOLON, "Expected ';' after value");

    return std::make_shared<PrintStmt>(exp);
}

StmtPtr Parser::parseReturnStmt()
{
    auto returnToken = previous();

    ExprPtr value;
    if (!check(TokenType::SEMICOLON)) {
        value = parseExpression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after return statement.");

    return std::make_shared<ReturnStmt>(returnToken, value);
}

StmtPtr Parser::parseExpressionStmt()
{
    auto exp = parseExpression();

    consume(TokenType::SEMICOLON, "Expected ';' after expression");

    return std::make_shared<ExpressionStmt>(exp);
}

StmtPtr Parser::parseStatement()
{
    if (match(TokenType::PRINT)) {
        return parsePrintStmt();
    }
    if (match(TokenType::LEFT_BRACE)) {
        return std::make_shared<BlockStmt>(parseBlock());
    }
    if (match(TokenType::IF)) {
        return parseIfStmt();
    }
    if (match(TokenType::WHILE)) {
        return parseWhileStmt();
    }
    if (match(TokenType::FOR)) {
        return parseForStmt();
    }
    if (match(TokenType::RETURN)) {
        return parseReturnStmt();
    }

    return parseExpressionStmt();
}

StmtPtr Parser::parseVarDeclaration()
{
    auto ident = consume(TokenType::IDENTIFIER, "Expected variable name.");

    ExprPtr init = nullptr;

    if (match(TokenType::EQUAL)) {
        init = parseExpression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");

    return std::make_shared<VarStmt>(ident, init);
}

StmtPtr Parser::parseFunction(const std::string& kind)
{
    auto name = consume(TokenType::IDENTIFIER, std::string{"Expected "} + kind + std::string{" name."});

    consume(TokenType::LEFT_PAREN, std::string{"Expected '(' after "} + kind + std::string{" name."});

    auto params = std::make_shared<std::vector<TokenPtr>>();
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (params->size() >= 255) {
                error(peek(), "Can't have more than 255 parameters.");
            }
            params->push_back(consume(TokenType::IDENTIFIER, "Expected parameter name."));
        } while (match(TokenType::COMMA));
    }

    consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");
    consume(TokenType::LEFT_BRACE, std::string{"Expected '{' before "} + kind + std::string{" body."});

    auto body = parseBlock();

    return std::make_shared<FunctionStmt>(name, params, body);
}

StmtPtr Parser::parseDeclaration()
{
    try {
        if (match(TokenType::VAR)) {
            return parseVarDeclaration();
        }
        if (match(TokenType::FUN)) {
            return parseFunction("function");
        }
        return parseStatement();

    } catch (parsing_error& error) {
        parsing_failed_ = true;

        synchronize();

        return nullptr;
    }
}

void Parser::synchronize()
{
    advance();

    while (!atEnd()) {
        if (previous()->tokenType_ == TokenType::SEMICOLON) return;

        switch (peek()->tokenType_) {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            default:
                break;
        }

        advance();
    }
}

std::optional<std::vector<StmtPtr>> Parser::parse()
{
    std::vector<StmtPtr> statements;
    while (!atEnd()) {
        statements.push_back(parseDeclaration());
    }

    if (parsing_failed_) return std::nullopt;

    return statements;
}

