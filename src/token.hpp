#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>

enum class TokenType {
    // Keywords
    BEGIN,
    END,
    FOR,
    IF,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

std::ostream& operator<<(std::ostream&, const Token&);

#endif
