#pragma once

#include <unordered_map>
#include <string>

#include "value.hpp"
#include "token.hpp"

struct Environment
{
    Environment() = default;
    Environment(std::shared_ptr<Environment> parent);

    void define(const std::string& name, LoxValuePtr value);

    LoxValuePtr get(TokenPtr token);

    void assign(TokenPtr token, LoxValuePtr value);

private:
    std::unordered_map<std::string, LoxValuePtr> values_;
    std::shared_ptr<Environment> parent_;

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

