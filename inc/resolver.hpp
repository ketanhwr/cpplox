#pragma once

#include <unordered_map>

#include "expr.hpp"
#include "stmt.hpp"

struct Interpreter;

struct Resolver: public Expr::AbstractVisitor, public Stmt::AbstractVisitor
{
    enum class FunctionType {
        NONE,
        FUNCTION
    };

    Resolver(Interpreter& interpreter);

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

    bool resolve(const std::vector<StmtPtr>& stmts);

private:
    void resolve(std::shared_ptr<std::vector<StmtPtr>> stmts);
    void resolve(StmtPtr stmt);
    void resolve(ExprPtr expr);
    void resolveLocal(ExprPtr expr, TokenPtr name);
    void resolveFunction(FunctionStmtPtr stmt, FunctionType type);
    void beginScope();
    void endScope();
    void declare(TokenPtr name);
    void define(TokenPtr name);

    Interpreter& interpreter_;
    bool has_error_{false};

    std::vector<std::unordered_map<std::string, bool>> scopes_;
    FunctionType currentFunction{FunctionType::NONE};
};

