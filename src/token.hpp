#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>

enum class TokenType : unsigned short {
    // clang-format off

    // Keywords
    ARRAY, BEGIN, CONST, DIV, DO, ELSE, ELSEIF, END, IF, MOD, MODULE, OF, OR,
    PROCEDURE, RECORD, THEN, TYPE, VAR, WHILE,

    // Literals
    IDENT, STRING, NUMBER,

    // Single-char tokens
    AND, COLON, COMMA, DOT, EQUAL, GREATER, HASH, LEFT_BRACKET, LEFT_PAREN,
    LESS, MINUS, PLUS, RIGHT_BRACKET, RIGHT_PAREN, SEMICOLON, STAR, TILDE,

    // Two-char tokens
    GREATER_EQUAL, LESS_EQUAL

    // clang-format on
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;

    std::string typeString() const;
};

std::ostream& operator<<(std::ostream&, const Token&);

#endif
