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
    GREATER_EQUAL, LESS_EQUAL, ASSIGN

    // clang-format on
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;

    [[nodiscard]] std::string typeString() const;

    /**
     * @brief For single-char tokens, returns the token type for a given character.
     * @param ch The character whose corresponding single-char token type should be returned.
     * @return The single-char token type corresponding token type should be returned
     * @throw invalid_argument exception if the given character does not correspond to
     * a single-char token type known to Oberon-0.
     */
    static TokenType typeFromChar(const char ch);
};

std::ostream& operator<<(std::ostream&, const Token&);

#endif
