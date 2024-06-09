#pragma once

#include "expr.hpp"

#include <sstream>

struct AstPrinter: public AbstractVisitor
{
    std::ostringstream result_;

    void visitBinaryExpr(BinaryExpr &exp) override;
    void visitGroupingExpr(GroupingExpr& expr) override;
    void visitLiteralExpr(LiteralExpr& expr) override;
    void visitUnaryExpr(UnaryExpr& expr) override;
};

