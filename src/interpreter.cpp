#include "interpreter.hpp"

#include <iostream>
#include <cmath>

#define CAST(TO_TYPE, FROM_VAL) std::dynamic_pointer_cast<TO_TYPE>(FROM_VAL)
#define EPS 1e-6

void Interpreter::visitBinaryExpr(BinaryExpr& expr)
{
    auto left = evaluate(expr.left_);
    auto right = evaluate(expr.right_);

    switch (expr.op_->tokenType_)
    {
        case TokenType::MINUS:
        {
            checkNumberOps(expr.op_, left, right);

            if (isFloat(left) || isFloat(right)) {
                auto val1 = getFloat(left), val2 = getFloat(right);
                result_ = std::make_shared<LoxFloat>(val1 - val2);
            } else {
                auto val1 = getInt(left), val2 = getInt(right);
                result_ = std::make_shared<LoxInteger>(val1 - val2);
            }

            break;
        }
        case TokenType::SLASH:
        {
            checkNumberOps(expr.op_, left, right);

            if (isFloat(left) || isFloat(right)) {
                auto val1 = getFloat(left), val2 = getFloat(right);

                if (std::abs(val2) < EPS) {
                    throw interpreter_error{expr.op_, "Division by 0"};
                }

                result_ = std::make_shared<LoxFloat>(val1 / val2);
            } else {
                auto val1 = getInt(left), val2 = getInt(right);

                if (val2 == 0) {
                    throw interpreter_error{expr.op_, "Division by 0"};
                }

                result_ = std::make_shared<LoxInteger>(val1 / val2);
            }

            break;
        }
        case TokenType::STAR:
        {
            checkNumberOps(expr.op_, left, right);

            if (isFloat(left) || isFloat(right)) {
                auto val1 = getFloat(left), val2 = getFloat(right);
                result_ = std::make_shared<LoxFloat>(val1 * val2);
            } else {
                auto val1 = getInt(left), val2 = getInt(right);
                result_ = std::make_shared<LoxInteger>(val1 * val2);
            }

            break;
        }
        case TokenType::PLUS:
        {
            if (isNum(left) && isNum(right)) {
                if (isFloat(left) || isFloat(right)) {
                    auto val1 = getFloat(left), val2 = getFloat(right);
                    result_ = std::make_shared<LoxFloat>(val1 + val2);
                } else {
                    auto val1 = getInt(left), val2 = getInt(right);
                    result_ = std::make_shared<LoxInteger>(val1 + val2);
                }
            } else if (isString(left) && isString(right)) {
                result_ = std::make_shared<LoxString>(CAST(LoxString, left)->value_ + CAST(LoxString, right)->value_);
            } else {
                throw new interpreter_error{expr.op_, "Operands must be both strings or numbers."};
            }

            break;
        }
        case TokenType::GREATER:
        {
            checkNumberOps(expr.op_, left, right);

            bool comp;

            if (isFloat(left) || isFloat(right)) {
                auto val1 = getFloat(left), val2 = getFloat(right);
                comp = (val1 > val2);
            } else {
                auto val1 = getInt(left), val2 = getInt(right);
                comp = (val1 > val2);
            }

            result_ = std::make_shared<LoxBool>(comp);
            break;
        }
        case TokenType::GREATER_EQUAL:
        {
            checkNumberOps(expr.op_, left, right);

            bool comp;

            if (isFloat(left) || isFloat(right)) {
                auto val1 = getFloat(left), val2 = getFloat(right);
                comp = (val1 >= val2);
            } else {
                auto val1 = getInt(left), val2 = getInt(right);
                comp = (val1 >= val2);
            }

            result_ = std::make_shared<LoxBool>(comp);
            break;
        }
        case TokenType::LESS:
        {
            checkNumberOps(expr.op_, left, right);

            bool comp;

            if (isFloat(left) || isFloat(right)) {
                auto val1 = getFloat(left), val2 = getFloat(right);
                comp = (val1 < val2);
            } else {
                auto val1 = getInt(left), val2 = getInt(right);
                comp = (val1 < val2);
            }

            result_ = std::make_shared<LoxBool>(comp);
            break;
        }
        case TokenType::LESS_EQUAL:
        {
            checkNumberOps(expr.op_, left, right);

            bool comp;

            if (isFloat(left) || isFloat(right)) {
                auto val1 = getFloat(left), val2 = getFloat(right);
                comp = (val1 <= val2);
            } else {
                auto val1 = getInt(left), val2 = getInt(right);
                comp = (val1 <= val2);
            }

            result_ = std::make_shared<LoxBool>(comp);
            break;
        }
        case TokenType::BANG_EQUAL:
        {
            result_ = std::make_shared<LoxBool>(!isEqual(left, right));
            break;
        }
        case TokenType::EQUAL_EQUAL:
        {
            result_ = std::make_shared<LoxBool>(isEqual(left, right));
            break;
        }
        default:
        {
            break;
        }
    }
}

void Interpreter::visitGroupingExpr(GroupingExpr& expr)
{
    result_ = evaluate(expr.expression_);
}

void Interpreter::visitLiteralExpr(LiteralExpr& expr)
{
    result_ = expr.value_;
}

void Interpreter::visitUnaryExpr(UnaryExpr& expr)
{
    auto right = evaluate(expr.right_);

    switch (expr.op_->tokenType_) {
        case TokenType::BANG:
        {
            result_ = std::make_shared<LoxBool>(!isTruthy(right));
            break;
        }
        case TokenType::MINUS:
        {
            checkNumberOp(expr.op_, right);

            if (isFloat(right)) {
                result_ = std::make_shared<LoxFloat>(-getFloat(right));
            } else {
                result_ = std::make_shared<LoxInteger>(-getInt(right));
            }

            break;
        }
        default: 
        {
            break;
        }
    }
}

bool Interpreter::isTruthy(LoxValuePtr value)
{
    if (auto boolVal = std::dynamic_pointer_cast<LoxBool>(value)) {
        return boolVal->value_;
    }

    if (auto numberVal = std::dynamic_pointer_cast<LoxInteger>(value)) {
        return numberVal->value_ != 0;
    }

    if (auto nilVal = std::dynamic_pointer_cast<LoxNil>(value)) {
        return false;
    }

    return true;
}

bool Interpreter::isEqual(LoxValuePtr a, LoxValuePtr b)
{
    if (auto nila = CAST(LoxNil, a)) {
        if (auto nilb = CAST(LoxNil, b)) {
            return true;
        }
        return false;
    } else if (auto nilb = CAST(LoxNil, b)) {
        return false;
    }

    if (isNum(a) && isNum(b)) {
        auto val1 = getFloat(a), val2 = getFloat(b);
        return std::abs(val1 - val2) < EPS;
    }

    if (isString(a) && isString(b)) {
        return CAST(LoxString, a)->value_ == CAST(LoxString, b)->value_;
    }

    if (isBool(a) && isBool(b)) {
        return CAST(LoxBool, a)->value_ == CAST(LoxBool, b)->value_;
    }

    return false;
}

bool Interpreter::isBool(LoxValuePtr value)
{
    if (auto boolVal = CAST(LoxBool, value)) {
        return true;
    }

    return false;
}

bool Interpreter::isNum(LoxValuePtr value)
{
    if (auto numVal = CAST(LoxInteger, value)) {
        return true;
    } else if (auto floatVal = CAST(LoxFloat, value)) {
        return true;
    }

    return false;
}

bool Interpreter::isFloat(LoxValuePtr value)
{
    if (auto floatVal = CAST(LoxFloat, value)) {
        return true;
    }

    return false;
}

bool Interpreter::isString(LoxValuePtr value)
{
    if (auto strVal = CAST(LoxString, value)) {
        return true;
    }
    return false;
}

int64_t Interpreter::getInt(LoxValuePtr value)
{
    return CAST(LoxInteger, value)->value_;
}

double Interpreter::getFloat(LoxValuePtr value)
{
    if (auto floatVal = CAST(LoxFloat, value)) {
        return floatVal->value_;
    }
    return CAST(LoxInteger, value)->value_;
}

void Interpreter::checkNumberOp(std::shared_ptr<Token> op, LoxValuePtr value)
{
    if (isNum(value)) return;
    throw new interpreter_error{op, "Operand must be a number."};
}

void Interpreter::checkNumberOps(std::shared_ptr<Token> op, LoxValuePtr left, LoxValuePtr right)
{
    if (isNum(left) && isNum(right)) return;
    throw new interpreter_error{op, "Operands must be numbers."};
}

LoxValuePtr Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
    expr->accept(*this);
    return result_;
}

void Interpreter::interpret(std::shared_ptr<Expr> expr)
{
    try {
        evaluate(expr);
        std::cout << "Output: " << *result_ << std::endl;
    } catch (interpreter_error& error) {
        std::cerr << "Line [" << error.token_->line_ << "]: " << error.what() << std::endl;
    }
}
