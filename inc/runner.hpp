#pragma once

#include <string>

#include "interpreter.hpp"

struct Runner
{
    Runner(bool repl_mode = false) : interpreter_{repl_mode} {}
    
    void runFromFile(const char *file);
    void runFromPrompt();

private:
    Interpreter interpreter_;
    std::string source_;

    void run();
};

