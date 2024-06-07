#include "scanner.hpp"

#include <iostream>

std::ostream& Token::operator<<(std::ostream& out)
{
    out << "Type [" << static_cast<uint8_t>(tokenType_) << ']';
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

char Scanner::advance()
{
    return program_[idx_++];
}

char Scanner::peek()
{
    return program_[idx_];
}

bool Scanner::check(char c)
{
    return (idx_ < program_.size() && peek() == c);
}

std::vector<Token> Scanner::scanTokens()
{
    std::vector<Token> tokens;

    while (idx_ != program_.size()) {
        char c = advance();
        switch (c) {
            // Single character tokens
            case '(': tokens.emplace_back(TokenType::LEFT_PAREN); break;
            case ')': tokens.emplace_back(TokenType::RIGHT_PAREN); break;
            case '{': tokens.emplace_back(TokenType::LEFT_BRACE); break;
            case '}': tokens.emplace_back(TokenType::RIGHT_BRACE); break;
            case ',': tokens.emplace_back(TokenType::COMMA); break;
            case '.': tokens.emplace_back(TokenType::DOT); break;
            case '-': tokens.emplace_back(TokenType::MINUS); break;
            case '+': tokens.emplace_back(TokenType::PLUS); break;
            case ';': tokens.emplace_back(TokenType::SEMICOLON); break;
            case '/': tokens.emplace_back(TokenType::SLASH); break;
            case '*': tokens.emplace_back(TokenType::STAR); break;
            
            // Single or double character tokens
            case '!': {
                if (check('=')) {
                    tokens.emplace_back(TokenType::BANG_EQUAL);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::BANG);
                }
                break;
            }
            case '=': {
                if (check('=')) {
                    tokens.emplace_back(TokenType::EQUAL_EQUAL);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::EQUAL);
                }
                break;
            }
            case '<': {
                if (check('=')) {
                    tokens.emplace_back(TokenType::LESS_EQUAL);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::LESS);
                }
                break;
            }
            case '>': {
                if (check('=')) {
                    tokens.emplace_back(TokenType::GREATER_EQUAL);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::GREATER);
                }
                break;
            }

            // Literals
            
        }
    }

    return tokens;
}

