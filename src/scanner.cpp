#include "scanner.hpp"

#include <iostream>
#include <unordered_map>

namespace {
    const static std::unordered_map<std::string, TokenType> reservedIdentifiers = {
        { "and", TokenType::AND},
        { "class", TokenType::CLASS},
        { "else", TokenType::ELSE},
        { "false", TokenType::FALSE},
        { "fun", TokenType::FUN},
        { "for", TokenType::FOR},
        { "if", TokenType::IF},
        { "nil", TokenType::NIL},
        { "or", TokenType::OR},
        { "print", TokenType::PRINT},
        { "return", TokenType::RETURN},
        { "super", TokenType::SUPER},
        { "this", TokenType::THIS},
        { "true", TokenType::TRUE},
        { "var", TokenType::VAR},
        { "while", TokenType::WHILE}
    };
}

Scanner::Scanner(const std::string& program)
    : program_{program}
{
}

bool Scanner::atEnd()
{
    return (current_ >= program_.size());
}

char Scanner::advance()
{
    return program_[current_++];
}

char Scanner::peek()
{
    if (atEnd()) return '\0';
    return program_[current_];
}

char Scanner::peekNext()
{
    if (current_ + 1 >= program_.size()) return '\0';
    return program_[current_ + 1];
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
    addToken(tokenType, nullptr);
}

void Scanner::parseString()
{
    while (peek() != '"' && !atEnd()) {
        if (peek() == '\n') {
            line_++;
        }

        advance();
    }

    if (atEnd()) {
        std::cerr << "Unterminated string" << std::endl;
        hasError_ = true;
        return;
    }

    advance();

    std::string value = program_.substr(start_ + 1, (current_ - start_ - 2));
    addToken(TokenType::STRING, std::make_shared<LoxString>(value));
}

void Scanner::parseNumber()
{
    bool isFloat = false;

    while (isNum(peek())) {
        advance();
    }

    if (peek() == '.' && isNum(peekNext())) {
        isFloat = true;
        advance();
        while (isNum(peek())) {
            advance();
        }
    }

    std::string value = program_.substr(start_, (current_ - start_));
    if (isFloat) {
        addToken(TokenType::NUMBER, std::make_shared<LoxFloat>(std::stod(value)));
    } else {
        addToken(TokenType::NUMBER, std::make_shared<LoxInteger>(std::stoll(value)));
    }
}

void Scanner::parseIdentifier()
{
    while (isAlphaNum(peek())) {
        advance();
    }

    std::string value = program_.substr(start_, (current_ - start_));

    TokenType tokenType;
    
    auto it = reservedIdentifiers.find(value);
    if (it != reservedIdentifiers.end()) {
        tokenType = it->second;
    } else {
        tokenType = TokenType::IDENTIFIER;
    }

    addToken(tokenType);
}

bool Scanner::isNum(char c)
{
    return (c >= '0' && c <= '9');
}

bool Scanner::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNum(char c)
{
    return isAlpha(c) || isNum(c);
}

std::optional<std::vector<std::shared_ptr<Token>>> Scanner::scanTokens()
{
    while (!atEnd()) {
        start_ = current_;

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

            case '/': {
                // Handle comments
                if (match('/')) {
                    while (!atEnd() && peek() != '\n') {
                        advance();
                    }
                } else if (match('*')) {
                    while (!(atEnd() || (peek() == '*' && peekNext() == '/'))) {
                        if (peek() == '\n') ++line_;

                        advance();
                    }
                    // We either reached end of file OR end of comment
                    if (!atEnd()) {
                        // Consume end of comment '*/'
                        advance();
                        advance();
                    }
                } else {
                    addToken(TokenType::SLASH); break;
                }
                break;
            }

            // Literals
            case '"': {
                parseString();
                break;
            }

            case ' ':
            case '\t':
            case '\r':
                break;
            
            case '\n': {
                line_++;
                break;
            }

            default: {
                if (isNum(c)) {
                    parseNumber();
                } else if (isAlpha(c)) {
                    parseIdentifier();
                } else {
                    std::cerr << "Unexpected character [" << c << "] at line number [" << line_ << ']' << std::endl;
                    hasError_ = true;
                }
            }
        }
    }

    if (hasError_) {
        return std::nullopt;
    } else {
        addToken(TokenType::END_OF_FILE);

        return tokens_;
    }
}

