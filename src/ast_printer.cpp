#include "ast_printer.hpp"

void AstPrinter::visitBinaryExpr(BinaryExpr &exp)
{
    std::ostringstream tempResult;

    tempResult << "(" << exp.op_->lexeme_ << " ";
    
    exp.left_->accept(*this);

    tempResult << result_.str() << " ";

    exp.right_->accept(*this);

    tempResult << result_.str() << ")";

    result_ = std::move(tempResult);
}

void AstPrinter::visitGroupingExpr(GroupingExpr& expr)
{
    std::ostringstream tempResult;

    tempResult << "(";

    expr.expression_->accept(*this);

    tempResult << result_.str() << ")";

    result_ = std::move(tempResult);
}

void AstPrinter::visitLiteralExpr(LiteralExpr& expr)
{
    std::ostringstream tempResult;

    tempResult << *expr.value_;
    
    result_ = std::move(tempResult);
}

void AstPrinter::visitUnaryExpr(UnaryExpr& expr)
{
    std::ostringstream tempResult;

    tempResult << "(" << expr.op_->lexeme_ << " ";

    expr.right_->accept(*this);

    tempResult << result_.str() << ")";

    result_ = std::move(tempResult);
}

