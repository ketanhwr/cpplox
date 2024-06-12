#pragma once

#include "expr.hpp"

#include <sstream>

struct AstPrinter: public Expr::AbstractVisitor
{
    std::ostringstream result_;

    void visitBinaryExpr(BinaryExprPtr expr) override;
    void visitGroupingExpr(GroupingExprPtr expr) override;
    void visitLiteralExpr(LiteralExprPtr expr) override;
    void visitUnaryExpr(UnaryExprPtr expr) override;
};

