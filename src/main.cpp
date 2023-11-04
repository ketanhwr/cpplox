#include <iostream>
#include <string>
#include <fstream>

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

enum class tokenType
{
    // Single character tokens
    LEFT_PAREN,
    RIGHT_PAREN,

    // Literals
    
    // Language Types

};

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <src-file>" << std::endl;
        return 1;
    }

    std::string fileContents = readFromFile(argv[1]);

    std::cout << "File contents: \n" << fileContents << std::endl;

    return 0;
}
