#include <iostream>
#include <string>
#include <fstream>

#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

void run(const std::string& program, bool repl_mode = false)
{
    Scanner scanner{program};

    auto tokens = scanner.scanTokens();

    if (!tokens) {
        return;
    }

    // for (auto& token: tokens.value()) {
    //     std::cout << *token << std::endl;
    // }

    Parser parser{tokens.value()};

    auto ast = parser.parse();

    if (ast) {
        // AstPrinter ast_printer;
        // ast->accept(ast_printer);

        // std::cout << ast_printer.result_.str() << std::endl;

        Interpreter interpreter{repl_mode};
        interpreter.interpret(ast.value());
    }
}

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

void runFromFile(const char *file)
{
    std::string fileContents{readFromFile(file)};

    // std::cout << "File contents: \n" << fileContents << std::endl;

    run(fileContents);
}

void runFromPrompt()
{
    std::cout << "Lox REPL" << std::endl;

    std::string line;

    std::cout << "lox> ";
    while (std::getline(std::cin, line)) {
        // std::cout << "You entered [" << line.size() << "] characters: " << line << std::endl;

        run(line, true);

        std::cout << "lox> ";
    }
}

int main(int argc, char **argv)
{
    if (argc > 2) {
        std::cout << "Usage: " << argv[0] << " [script]" << std::endl;
        return 1;
    } else if (argc == 2) {
        runFromFile(argv[1]);
    } else {
        runFromPrompt();
    }

    return 0;
}
