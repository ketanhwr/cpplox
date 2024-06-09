#pragma once

#include "expr.hpp"

using LoxValuePtr = std::shared_ptr<LoxValue>;

struct Interpreter: public AbstractVisitor
{
    struct interpreter_error: public std::runtime_error
    {
        std::shared_ptr<Token> token_;

        interpreter_error(std::shared_ptr<Token> token, const std::string& msg)
            : token_{token}
            , std::runtime_error{msg}
        {}
    };

    LoxValuePtr result_;

    // Visitor methods
    void visitBinaryExpr(BinaryExpr& expr) override;
    void visitGroupingExpr(GroupingExpr& expr) override;
    void visitLiteralExpr(LiteralExpr& expr) override;
    void visitUnaryExpr(UnaryExpr& expr) override;

    // Helpers
    bool isTruthy(LoxValuePtr value);
    bool isEqual(LoxValuePtr a, LoxValuePtr b);

    bool isBool(LoxValuePtr value);
    bool isNum(LoxValuePtr value);
    bool isFloat(LoxValuePtr value);
    bool isString(LoxValuePtr value);

    int64_t getInt(LoxValuePtr value);
    double getFloat(LoxValuePtr value);

    // Checkers
    void checkNumberOp(std::shared_ptr<Token> op, LoxValuePtr value);
    void checkNumberOps(std::shared_ptr<Token> op, LoxValuePtr left, LoxValuePtr right);

    LoxValuePtr evaluate(std::shared_ptr<Expr> expr);

    void interpret(std::shared_ptr<Expr>);
};

