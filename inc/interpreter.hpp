#pragma once

#include "expr.hpp"
#include "stmt.hpp"
#include "environment.hpp"

#include <vector>

struct Interpreter: public Expr::AbstractVisitor, public Stmt::AbstractVisitor
{
    Interpreter(bool repl_mode = false);

    bool repl_mode_;
    LoxValuePtr result_;

    std::shared_ptr<Environment> env_;

    // Visitor methods for Expressions
    void visitAssignExpr(AssignExpr& expr) override;
    void visitBinaryExpr(BinaryExpr& expr) override;
    void visitGroupingExpr(GroupingExpr& expr) override;
    void visitLiteralExpr(LiteralExpr& expr) override;
    void visitUnaryExpr(UnaryExpr& expr) override;
    void visitVariableExpr(VariableExpr& expr) override;

    // Visitor methods for Statements
    void visitIfStmt(IfStmt& stmt) override;
    void visitBlockStmt(BlockStmt& stmt) override;
    void visitExpressionStmt(ExpressionStmt& stmt) override;
    void visitPrintStmt(PrintStmt& stmt) override;
    void visitVarStmt(VarStmt& stmt) override;

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
    void execute(std::shared_ptr<Stmt> stmt);
    void executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements);

    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
};

