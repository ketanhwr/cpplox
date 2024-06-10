#include "parser.hpp"

#include <iostream>

Parser::Parser(const std::vector<std::shared_ptr<Token>>& tokens)
    : tokens_{tokens}
{}

auto Parser::error(std::shared_ptr<Token> token, const std::string& msg) -> parsing_error
{
    if (token->tokenType_ == TokenType::END_OF_FILE) {
        std::cerr << "Line [" << token->line_ << "] Error at end: " << msg << std::endl;
    } else {
        std::cerr << "Line [" << token->line_ << "] Error at " << token->lexeme_ << ": " << msg << std::endl;
    }

    return parsing_error{""};
}

std::shared_ptr<Token> Parser::peek()
{
    return tokens_[current_];
}

std::shared_ptr<Token> Parser::previous()
{
    return tokens_[current_ - 1];
}

std::shared_ptr<Token> Parser::advance()
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

std::shared_ptr<Token> Parser::consume(TokenType tokenType, const std::string& msg)
{
    if (check(tokenType)) return advance();

    throw error(peek(), msg);
}

std::shared_ptr<Expr> Parser::parseExpression()
{
    return parseEquality();
}

std::shared_ptr<Expr> Parser::parseEquality()
{
    auto expr = parseComparison();

    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        auto op = previous();
        auto right = parseComparison();

        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::parseComparison()
{
    auto expr = parseTerm();

    while (match(TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREATER, TokenType::GREATER_EQUAL)) {
        auto op = previous();
        auto right = parseTerm();

        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::parseTerm()
{
    auto expr = parseFactor();

    while (match(TokenType::MINUS, TokenType::PLUS)) {
        auto op = previous();
        auto right = parseFactor();

        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::parseFactor()
{
    auto expr = parseUnary();

    while (match(TokenType::STAR, TokenType::SLASH)) {
        auto op = previous();
        auto right = parseUnary();

        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::parseUnary()
{
    if (match(TokenType::BANG, TokenType::MINUS)) {
        auto op = previous();
        auto right = parseUnary();

        return std::make_shared<UnaryExpr>(op, right);
    }

    return parsePrimary();
}

std::shared_ptr<Expr> Parser::parsePrimary()
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

    throw error(peek(), "Expected expression");
}

std::shared_ptr<Stmt> Parser::parsePrintStmt()
{
    auto exp = parseExpression();

    consume(TokenType::SEMICOLON, "Expected ';' after value");

    return std::make_shared<PrintStmt>(exp);
}

std::shared_ptr<Stmt> Parser::parseExpressionStmt()
{
    auto exp = parseExpression();

    consume(TokenType::SEMICOLON, "Expected ';' after expression");

    return std::make_shared<ExpressionStmt>(exp);
}

std::shared_ptr<Stmt> Parser::parseStatement()
{
    if (match(TokenType::PRINT)) {
        return parsePrintStmt();
    }

    return parseExpressionStmt();
}

std::optional<std::vector<std::shared_ptr<Stmt>>> Parser::parse()
{
    // try {
    //     return parseExpression();
    // } catch (parsing_error& error) {
    //     return nullptr;
    // }
    //
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!atEnd()) {
        statements.push_back(parseStatement());
    }

    return statements;
}

