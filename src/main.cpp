#include <iostream>

#include "runner.hpp"

int main(int argc, char **argv)
{
    if (argc > 2) {
        std::cout << "Usage: " << argv[0] << " [script]" << std::endl;
        return 1;
    } else if (argc == 2) {
        Runner runner;
        runner.runFromFile(argv[1]);
    } else {
        Runner runner{/* repl_mode = */ true};
        runner.runFromPrompt();
    }

    return 0;
}
