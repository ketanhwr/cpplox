#pragma once

#include <unordered_map>
#include <string>

#include "value.hpp"
#include "token.hpp"

struct Environment: public std::enable_shared_from_this<Environment>
{
    Environment() = default;
    Environment(std::shared_ptr<Environment> parent);

    void define(const std::string& name, LoxValuePtr value);

    LoxValuePtr get(TokenPtr token);
    LoxValuePtr getAt(int distance, const std::string& name);

    void assign(TokenPtr token, LoxValuePtr value);
    void assignAt(int distance, TokenPtr token, LoxValuePtr value);

private:
    std::unordered_map<std::string, LoxValuePtr> values_;
    std::shared_ptr<Environment> parent_;

    std::shared_ptr<Environment> ancestor(int distance);
};

struct EnvGuard
{
    EnvGuard(std::shared_ptr<Environment>& currentEnv, std::shared_ptr<Environment> oldEnv)
        : newEnv_{currentEnv}, oldEnv_{oldEnv}
    {}

    ~EnvGuard() {
        newEnv_ = oldEnv_;
    }

private:
    std::shared_ptr<Environment>& newEnv_;
    std::shared_ptr<Environment> oldEnv_;
};

