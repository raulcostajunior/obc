#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>

enum class TokenType : unsigned short {
    // clang-format off

    // Keywords
    ARRAY, BEGIN, CONST, DIV, DO, ELSE, ELSEIF, END, IF, MOD, MODULE, OF, OR,
    PROCEDURE, RECORD, THEN, TYPE, VAR, WHILE, TRUE, FALSE, NIL, FOR, RETURN,
    REPEAT, CASE, UNTIL, IMPORT, POINTER, IN, IS, TO,

    // Literals
    IDENT, STRING, INTEGER, REAL,

    // Single-char tokens
    AND, COLON, COMMA, DOT, EQUAL, GREATER, HASH, LEFT_BRACKET, LEFT_PAREN,
    LESS, MINUS, PLUS, RIGHT_BRACKET, RIGHT_PAREN, SEMICOLON, STAR, TILDE,
    CIRCUMFLEX, LEFT_CURLY, RIGHT_CURLY,

    // Two-char tokens
    GREATER_EQUAL, LESS_EQUAL, ASSIGN, LABEL_RANGE

    // clang-format on
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;

    [[nodiscard]] std::string typeString() const;

    /**
     * @brief For single-char tokens, returns the token type for a given character.
     *
     * @param chr The character whose corresponding single-char token type should be returned.
     * @return The single-char token type corresponding token type should be returned
     *
     * @throw invalid_argument exception if the given character does not correspond to
     * a single-char token type known to Oberon-0.
     */
    static TokenType typeFromChar(char chr);

    /**
     * @brief Returns the token type of the keyword that corresponds to a given lexeme.
     *
     * @attention internally this method works with case-sensitive comparisons and the input
     * lexeme must be all upper-case as this is how the Oberon language specifies its keywords.
     * Any desired support for lowercase (or mixed case) keywords must be provided by the
     * callers of this method. For example, the caller can (and should) convert the lexeme to
     * upper-case before calling this method.
     *
     * @param lex the lexeme whose keyword token type should be returned.
     * @return the keyword token type corresponding to the lexeme.
     *
     * @throw invalid_argument when the given lexeme doesn't match any of the keyword token
     * types of the language.
     */
    static TokenType keywordTypeFromLexeme(const std::string& lex);
};

std::ostream& operator<<(std::ostream&, const Token&);

#endif
