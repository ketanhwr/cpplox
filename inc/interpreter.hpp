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

    std::shared_ptr<Environment> global_;
    std::shared_ptr<Environment> env_;

    // Visitor methods for Expressions
    void visitAssignExpr(AssignExprPtr expr) override;
    void visitBinaryExpr(BinaryExprPtr expr) override;
    void visitGroupingExpr(GroupingExprPtr expr) override;
    void visitLiteralExpr(LiteralExprPtr expr) override;
    void visitUnaryExpr(UnaryExprPtr expr) override;
    void visitVariableExpr(VariableExprPtr expr) override;
    void visitLogicalExpr(LogicalExprPtr expr) override;
    void visitCallExpr(CallExprPtr expr) override;

    // Visitor methods for Statements
    void visitWhileStmt(WhileStmtPtr stmt) override;
    void visitIfStmt(IfStmtPtr stmt) override;
    void visitBlockStmt(BlockStmtPtr stmt) override;
    void visitExpressionStmt(ExpressionStmtPtr stmt) override;
    void visitPrintStmt(PrintStmtPtr stmt) override;
    void visitVarStmt(VarStmtPtr stmt) override;
    void visitFunctionStmt(FunctionStmtPtr stmt) override;
    void visitReturnStmt(ReturnStmtPtr stmt) override;

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
    void checkNumberOp(TokenPtr op, LoxValuePtr value);
    void checkNumberOps(TokenPtr op, LoxValuePtr left, LoxValuePtr right);

    LoxValuePtr evaluate(ExprPtr expr);
    void execute(StmtPtr stmt);
    void executeBlock(std::shared_ptr<std::vector<StmtPtr>> statements, std::shared_ptr<Environment> env);

    void interpret(const std::vector<StmtPtr>& statements);
};

