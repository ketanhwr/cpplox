#pragma once

#include "expr.hpp"

#include <sstream>

struct AstPrinter: public Expr::AbstractVisitor
{
    std::ostringstream result_;

    void visitBinaryExpr(BinaryExpr& expr) override;
    void visitGroupingExpr(GroupingExpr& expr) override;
    void visitLiteralExpr(LiteralExpr& expr) override;
    void visitUnaryExpr(UnaryExpr& expr) override;
};

