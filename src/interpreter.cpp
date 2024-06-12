#include "interpreter.hpp"

#include "lox_exception.hpp"
#include "native_clock.hpp"
#include "function.hpp"

#include <iostream>
#include <cmath>

#define CAST(TO_TYPE, FROM_VAL) std::dynamic_pointer_cast<TO_TYPE>(FROM_VAL)
#define EPS 1e-6

Interpreter::Interpreter(bool repl_mode)
    : repl_mode_{repl_mode}
    , result_{std::make_shared<LoxNil>()}
    , global_{std::make_shared<Environment>()}
    , env_{global_}
{
    global_->define("clock", std::make_shared<NativeClock>());
}

void Interpreter::visitAssignExpr(AssignExprPtr expr)
{
    auto right = evaluate(expr->value_);

    env_->assign(expr->name_, result_);
}

void Interpreter::visitBinaryExpr(BinaryExprPtr expr)
{
    auto left = evaluate(expr->left_);
    auto right = evaluate(expr->right_);

    switch (expr->op_->tokenType_)
    {
        case TokenType::MINUS:
        {
            checkNumberOps(expr->op_, left, right);

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
            checkNumberOps(expr->op_, left, right);

            if (isFloat(left) || isFloat(right)) {
                auto val1 = getFloat(left), val2 = getFloat(right);

                if (std::abs(val2) < EPS) {
                    throw interpreter_error{expr->op_, "Division by 0"};
                }

                result_ = std::make_shared<LoxFloat>(val1 / val2);
            } else {
                auto val1 = getInt(left), val2 = getInt(right);

                if (val2 == 0) {
                    throw interpreter_error{expr->op_, "Division by 0"};
                }

                result_ = std::make_shared<LoxInteger>(val1 / val2);
            }

            break;
        }
        case TokenType::STAR:
        {
            checkNumberOps(expr->op_, left, right);

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
                throw interpreter_error{expr->op_, "Operands must be both strings or numbers."};
            }

            break;
        }
        case TokenType::GREATER:
        {
            checkNumberOps(expr->op_, left, right);

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
            checkNumberOps(expr->op_, left, right);

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
            checkNumberOps(expr->op_, left, right);

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
            checkNumberOps(expr->op_, left, right);

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

void Interpreter::visitGroupingExpr(GroupingExprPtr expr)
{
    result_ = evaluate(expr->expression_);
}

void Interpreter::visitLiteralExpr(LiteralExprPtr expr)
{
    result_ = expr->value_;
}

void Interpreter::visitUnaryExpr(UnaryExprPtr expr)
{
    auto right = evaluate(expr->right_);

    switch (expr->op_->tokenType_) {
        case TokenType::BANG:
        {
            result_ = std::make_shared<LoxBool>(!isTruthy(right));
            break;
        }
        case TokenType::MINUS:
        {
            checkNumberOp(expr->op_, right);

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

void Interpreter::visitVariableExpr(VariableExprPtr expr)
{
    result_ = env_->get(expr->name_);
}

void Interpreter::visitLogicalExpr(LogicalExprPtr expr)
{
    auto left = evaluate(expr->left_);

    if (expr->op_->tokenType_ == TokenType::OR) {
        if (isTruthy(left)) {
            return;
        }
    } else if (expr->op_->tokenType_ == TokenType::AND) {
        if (!isTruthy(left)) {
            return;
        }
    }

    evaluate(expr->right_);
}

void Interpreter::visitCallExpr(CallExprPtr expr)
{
    auto callee = evaluate(expr->callee_);

    std::vector<LoxValuePtr> args;
    for (auto arg: *(expr->args_)) {
        args.push_back(evaluate(arg));
    }

    if (auto function = CAST(LoxCallable, callee)) {
        if (args.size() != function->arity()) {
            std::string errorMsg_{"Expected "};
            errorMsg_.append(std::to_string(function->arity()));
            errorMsg_.append(" argument(s) but got ");
            errorMsg_.append(std::to_string(args.size()));
            errorMsg_.append(1, '.');

            throw interpreter_error{expr->paren_, errorMsg_};
        }

        result_ = function->call(*this, args);
    } else {
        throw interpreter_error{expr->paren_, "Can only call functions and classes."};
    }
}

void Interpreter::visitWhileStmt(WhileStmtPtr stmt)
{
    while (isTruthy(evaluate(stmt->condition_))) {
        execute(stmt->statements_);
    }
}

void Interpreter::visitIfStmt(IfStmtPtr stmt)
{
    if (isTruthy(evaluate(stmt->condition_))) {
        execute(stmt->thenStmt_);
    } else if (stmt->elseStmt_) {
        execute(stmt->elseStmt_);
    }
}

void Interpreter::visitBlockStmt(BlockStmtPtr stmt)
{
    executeBlock(stmt->statements_, std::make_shared<Environment>(env_));
}

void Interpreter::visitExpressionStmt(ExpressionStmtPtr stmt)
{
    evaluate(stmt->expression_);
}

void Interpreter::visitPrintStmt(PrintStmtPtr stmt)
{
    auto value = evaluate(stmt->expression_);

    std::cout << *value << std::endl;
}

void Interpreter::visitVarStmt(VarStmtPtr stmt)
{
    LoxValuePtr initVal;
    if (stmt->initializer_) {
        initVal = evaluate(stmt->initializer_);
    } else {
        initVal = std::make_shared<LoxNil>();
    }

    env_->define(stmt->name_->lexeme_, initVal);
}

void Interpreter::visitFunctionStmt(FunctionStmtPtr stmt)
{
    auto funcDef = std::make_shared<LoxFunction>(stmt);
    env_->define(stmt->name_->lexeme_, funcDef);
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

void Interpreter::checkNumberOp(TokenPtr op, LoxValuePtr value)
{
    if (isNum(value)) return;
    throw interpreter_error{op, "Operand must be a number."};
}

void Interpreter::checkNumberOps(TokenPtr op, LoxValuePtr left, LoxValuePtr right)
{
    if (isNum(left) && isNum(right)) return;
    throw interpreter_error{op, "Operands must be numbers."};
}

LoxValuePtr Interpreter::evaluate(ExprPtr expr)
{
    expr->accept(*this);
    return result_;
}

void Interpreter::execute(StmtPtr stmt)
{
    stmt->accept(*this);

    if (repl_mode_) {
        std::cout << *result_ << std::endl;
    }
}

void Interpreter::executeBlock(std::shared_ptr<std::vector<StmtPtr>> statements, std::shared_ptr<Environment> env)
{
    auto previousEnv = env_;
    env_ = env;

    EnvGuard envGuard{env_, previousEnv};

    for (auto stmt: *statements) {
        stmt->accept(*this);
    }
}

void Interpreter::interpret(const std::vector<StmtPtr>& statements)
{
    try {
        for (auto statement: statements) {
            execute(statement);
        }
    } catch (interpreter_error& error) {
        std::cerr << "Line [" << error.token_->line_ << "]: " << error.what() << std::endl;
    }
}

