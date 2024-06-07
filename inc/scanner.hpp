#pragma once

#include <vector>
#include <string>
#include <variant>

enum class TokenType
{
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    LESS, LESS_EQUAL,
    GREATER, GREATER_EQUAL,

    IDENTIFIER, STRING, NUMBER,

    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NUL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    END_OF_FILE,
};

struct Token {
    TokenType tokenType_;
    std::variant<std::string, double> value_;

    Token(TokenType tokenType);

    template <typename T>
    Token(TokenType tokenType, T value) : tokenType_{tokenType}, value_{value} {}

    friend std::ostream& operator<<(std::ostream& out, Token& token);
};

class Scanner
{
    const std::string& program_;
    std::vector<Token> tokens_;

    size_t current_{0};
    size_t start_{0};

    bool atEnd();

    char advance();
    char peek();
    char peekNext();

    bool match(char c);
    
    void addToken(TokenType tokenType);

    template <typename T>
    void addToken(TokenType tokenType, T value)
    {
        tokens_.emplace_back(tokenType, value);
    }

    void parseString();
    void parseNumber();
    void parseIdentifier();

    bool isNum(char c);
    bool isAlpha(char c);
    bool isAlphaNum(char c);

public:
    Scanner(const std::string& program);

    std::vector<Token> scanTokens();
};

