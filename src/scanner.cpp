#include "scanner.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& out, Token& token)
{
    out << "Type [" << static_cast<int>(token.tokenType_) << ']';
    return out;
}

Token::Token(TokenType tokenType)
    : tokenType_{tokenType}
{
}

Scanner::Scanner(const std::string& program)
    : program_{program}
{
}

bool Scanner::atEnd()
{
    return (idx_ >= program_.size());
}

char Scanner::advance()
{
    return program_[idx_++];
}

char Scanner::peek()
{
    return program_[idx_];
}

bool Scanner::match(char c)
{
    if (atEnd()) return false;
    if (peek() == c) {
        advance();
        return true;
    }

    return false;
}

void Scanner::addToken(TokenType tokenType)
{
    tokens_.emplace_back(tokenType);
}

std::vector<Token> Scanner::scanTokens()
{
    while (idx_ != program_.size()) {
        char c = advance();
        switch (c) {
            // Single character tokens
            case '(': addToken(TokenType::LEFT_PAREN); break;
            case ')': addToken(TokenType::RIGHT_PAREN); break;
            case '{': addToken(TokenType::LEFT_BRACE); break;
            case '}': addToken(TokenType::RIGHT_BRACE); break;
            case ',': addToken(TokenType::COMMA); break;
            case '.': addToken(TokenType::DOT); break;
            case '-': addToken(TokenType::MINUS); break;
            case '+': addToken(TokenType::PLUS); break;
            case ';': addToken(TokenType::SEMICOLON); break;
            case '/': addToken(TokenType::SLASH); break;
            case '*': addToken(TokenType::STAR); break;
            
            // Single or double character tokens
            case '!': {
                addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
                break;
            }
            case '=': {
                addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
                break;
            }
            case '<': {
                addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
                break;
            }
            case '>': {
                addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
                break;
            }

            // Literals
            
        }
    }

    return tokens_;
}

