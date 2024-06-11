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
    return parseAssignment();
}

std::shared_ptr<Expr> Parser::parseAssignment()
{
    auto expr = parseEquality();

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

    if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<VariableExpr>(previous());
    }

    throw error(peek(), "Expected expression");
}

std::shared_ptr<Stmt> Parser::parseWhileStmt()
{
    consume(TokenType::LEFT_PAREN, "Expected '(' after while");

    auto cond = parseExpression();

    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition");

    auto whileBlock = parseStatement();

    return std::make_shared<WhileStmt>(cond, whileBlock);
}

std::shared_ptr<Stmt> Parser::parseIfStmt()
{
    consume(TokenType::LEFT_PAREN, "Expected '(' after if");

    auto cond = parseExpression();
    
    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition");

    auto thenBlock = parseStatement();

    std::shared_ptr<Stmt> elseBlock;
    if (match(TokenType::ELSE)) {
        elseBlock = parseStatement();
    }

    return std::make_shared<IfStmt>(cond, thenBlock, elseBlock);
}

std::shared_ptr<Stmt> Parser::parseBlock()
{
    auto statements = std::make_shared<std::vector<std::shared_ptr<Stmt>>>();

    while (!check(TokenType::RIGHT_BRACE) && !atEnd()) {
        statements->push_back(parseDeclaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expected '}' after block");

    return std::make_shared<BlockStmt>(statements);
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
    if (match(TokenType::LEFT_BRACE)) {
        return parseBlock();
    }
    if (match(TokenType::IF)) {
        return parseIfStmt();
    }
    if (match(TokenType::WHILE)) {
        return parseWhileStmt();
    }

    return parseExpressionStmt();
}

std::shared_ptr<Stmt> Parser::parseVarDeclaration()
{
    auto ident = consume(TokenType::IDENTIFIER, "Expected variable name.");

    std::shared_ptr<Expr> init = nullptr;

    if (match(TokenType::EQUAL)) {
        init = parseExpression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");

    return std::make_shared<VarStmt>(ident, init);
}

std::shared_ptr<Stmt> Parser::parseDeclaration()
{
    try {
        if (match(TokenType::VAR)) {
            return parseVarDeclaration();
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

std::optional<std::vector<std::shared_ptr<Stmt>>> Parser::parse()
{
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!atEnd()) {
        statements.push_back(parseDeclaration());
    }

    if (parsing_failed_) return std::nullopt;

    return statements;
}

