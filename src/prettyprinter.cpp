#include "../build/expr.hpp"

#include <iostream>
#include <sstream>

struct AstPrinter: public AbstractVisitor
{
    std::ostringstream result_;

    void visitBinaryExpr(BinaryExpr &exp) override
    {
        std::ostringstream tempResult;

        tempResult << "(" << exp.op_.lexeme_ << " ";
        
        exp.left_.accept(*this);

        tempResult << result_.str() << " ";

        exp.right_.accept(*this);

        tempResult << result_.str() << ")";

        result_ = std::move(tempResult);
    }

    void visitGroupingExpr(GroupingExpr& expr) override
    {
        std::ostringstream tempResult;

        tempResult << "(";

        expr.expression_.accept(*this);

        tempResult << result_.str() << ")";

        result_ = std::move(tempResult);
    }

    void visitLiteralExpr(LiteralExpr& expr) override
    {
        std::ostringstream tempResult;

        tempResult << expr.value_;
        
        result_ = std::move(tempResult);
    }

    void visitUnaryExpr(UnaryExpr& expr) override
    {
        std::ostringstream tempResult;

        tempResult << "(" << expr.op_.lexeme_ << " ";

        expr.right_.accept(*this);

        tempResult << result_.str() << ")";

        result_ = std::move(tempResult);
    }
};

int main()
{
    Expr* expr = new BinaryExpr(
            *(new UnaryExpr(
                *(new Token(TokenType::MINUS, nullptr, "-", 1)),
                *(new LiteralExpr(*(new LoxInteger(45)))))),
            *(new Token(TokenType::STAR, nullptr, "*", 1)),
            *(new GroupingExpr(
                *(new LiteralExpr(*(new LoxFloat(5.5))))
                ))
        );

    AstPrinter printer;

    expr->accept(printer);
    std::cout << printer.result_.str() << std::endl;
    
    return 0;
}

