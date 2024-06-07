#pragma once

#include <vector>
#include <string>

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

    Token(TokenType tokenType);

    friend std::ostream& operator<<(std::ostream& out, Token& token);
};

class Scanner
{
    const std::string& program_;
    size_t idx_{0};

    bool atEnd();

    char advance();
    char peek();

    bool match(char c);
    
    void addToken(TokenType tokenType);

    std::vector<Token> tokens_;

public:
    Scanner(const std::string& program);

    std::vector<Token> scanTokens();
};

