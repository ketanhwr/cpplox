#include "runner.hpp"

#include <fstream>
#include <iostream>

#include "scanner.hpp"
#include "parser.hpp"

namespace
{
    std::string readFromFile(const char* fileName)
    {
        std::ifstream file{fileName, std::ios_base::in | std::ios_base::binary};

        if (!file.is_open()) {
            std::cerr << "Could not open file: " << fileName << std::endl;
            throw;
        }

        std::string contents;
        file.seekg(0, std::ios_base::end);
        contents.resize(file.tellg());
        file.seekg(0, std::ios_base::beg);
        file.read(&contents[0], contents.size());

        return contents;
    }
}

void Runner::run()
{
    Scanner scanner{source_};

    auto tokens = scanner.scanTokens();

    if (!tokens) {
        return;
    }

    Parser parser{tokens.value()};

    auto ast = parser.parse();

    if (ast) {
        interpreter_.interpret(ast.value());
    }
}

void Runner::runFromFile(const char *file)
{
    source_ = readFromFile(file);
    run();
}

void Runner::runFromPrompt()
{
    std::cout << "Lox REPL" << std::endl;

    std::string line;

    std::cout << "lox> ";
    while (std::getline(std::cin, source_)) {
        run();

        std::cout << "lox> ";
    }
}

