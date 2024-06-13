#include "resolver.hpp"

#include <iostream>

#include "interpreter.hpp"

Resolver::Resolver(Interpreter& interpreter)
    : interpreter_{interpreter}
{ }

void Resolver::visitAssignExpr(AssignExprPtr expr)
{
    resolve(expr->value_);
    resolveLocal(expr, expr->name_);
}

void Resolver::visitBinaryExpr(BinaryExprPtr expr)
{
    resolve(expr->left_);
    resolve(expr->right_);
}

void Resolver::visitGroupingExpr(GroupingExprPtr expr)
{
    resolve(expr->expression_);
}

void Resolver::visitLiteralExpr(LiteralExprPtr expr)
{
    return;
}

void Resolver::visitUnaryExpr(UnaryExprPtr expr)
{
    resolve(expr->right_);
}

void Resolver::visitVariableExpr(VariableExprPtr expr)
{
    if (!scopes_.empty()) {
        auto it = scopes_.back().find(expr->name_->lexeme_);
        if (it != scopes_.back().end() && !it->second) {
            std::cerr << "Line [" << expr->name_->line_ << "]: Can't read local variable in its own initializer." << std::endl;
            has_error_ = true;
        }
    }

    resolveLocal(expr, expr->name_);
}

void Resolver::visitLogicalExpr(LogicalExprPtr expr)
{
    resolve(expr->left_);
    resolve(expr->right_);
}

void Resolver::visitCallExpr(CallExprPtr expr)
{
    resolve(expr->callee_);

    for (auto args: *expr->args_) {
        resolve(args);
    }
}

void Resolver::visitWhileStmt(WhileStmtPtr stmt)
{
    resolve(stmt->condition_);
    resolve(stmt->statements_);
}

void Resolver::visitIfStmt(IfStmtPtr stmt)
{
    resolve(stmt->condition_);
    resolve(stmt->thenStmt_);

    if (stmt->elseStmt_) {
        resolve(stmt->elseStmt_);
    }
}

void Resolver::visitBlockStmt(BlockStmtPtr stmt)
{
    beginScope();
    resolve(stmt->statements_);
    endScope();
}

void Resolver::visitExpressionStmt(ExpressionStmtPtr stmt)
{
    resolve(stmt->expression_);
}

void Resolver::visitPrintStmt(PrintStmtPtr stmt)
{
    resolve(stmt->expression_);
}

void Resolver::visitVarStmt(VarStmtPtr stmt)
{
    declare(stmt->name_);
    
    if (stmt->initializer_) {
        resolve(stmt->initializer_);
    }

    define(stmt->name_);
}

void Resolver::visitFunctionStmt(FunctionStmtPtr stmt)
{
    declare(stmt->name_);
    define(stmt->name_);

    resolveFunction(stmt, FunctionType::FUNCTION);
}

void Resolver::visitReturnStmt(ReturnStmtPtr stmt)
{
    if (currentFunction == FunctionType::NONE) {
        std::cerr << "Line [" << stmt->keyword_->line_ << "]: Can't return from top-level code." << std::endl;
        has_error_ = true;
    }

    if (stmt->value_) {
        resolve(stmt->value_);
    }
}

void Resolver::resolve(std::shared_ptr<std::vector<StmtPtr>> stmts)
{
    for (auto stmt: *stmts) {
        resolve(stmt);
    }
}

void Resolver::resolve(StmtPtr stmt)
{
    stmt->accept(*this);
}

void Resolver::resolve(ExprPtr expr)
{
    expr->accept(*this);
}

void Resolver::resolveLocal(ExprPtr expr, TokenPtr name)
{
    for (int i = scopes_.size() - 1; i >= 0; --i) {
        if (scopes_.at(i).contains(name->lexeme_)) {
            interpreter_.resolve(expr, scopes_.size() - i - 1);
            return;
        }
    }
}

void Resolver::resolveFunction(FunctionStmtPtr stmt, FunctionType type)
{
    auto previousType = currentFunction;
    currentFunction = type;

    beginScope();

    for (auto param: *stmt->params_) {
        declare(param);
        define(param);
    }
    resolve(stmt->body_);

    endScope();

    currentFunction = previousType;
}

void Resolver::beginScope()
{
    scopes_.push_back({});
}

void Resolver::endScope()
{
    scopes_.pop_back();
}

void Resolver::declare(TokenPtr name)
{
    if (scopes_.empty()) return;

    auto& scope = scopes_.back();

    if (scope.contains(name->lexeme_)) {
        std::cerr << "Line [" << name->line_ << "]: Already a variable with this name in this scope" << std::endl;
        has_error_ = true;
    }

    scope.insert({name->lexeme_, false});
}

void Resolver::define(TokenPtr name)
{
    if (scopes_.empty()) return;
    scopes_.back().insert_or_assign(name->lexeme_, true);
}

bool Resolver::resolve(const std::vector<StmtPtr>& stmts)
{
    for (auto stmt: stmts) {
        resolve(stmt);
    }

    return !has_error_;
}

