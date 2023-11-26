#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>

enum class TokenType : unsigned short {
    // clang-format off

    // Keywords
    ARRAY, BEGIN, BY, CONST, DIV, DO, ELSE, ELSEIF, END, IF, MOD, MODULE, OF, OR,
    PROCEDURE, RECORD, THEN, TYPE, VAR, WHILE, TRUE, FALSE, NIL, FOR, RETURN,
    REPEAT, CASE, UNTIL, IMPORT, POINTER, IN, IS, TO,

    // Literals
    IDENT, STRING, INTEGER, REAL,

    // Single-char tokens
    AND, COLON, COMMA, DOT, EQUAL, GREATER, HASH, LEFT_BRACKET, LEFT_PAREN,
    LESS, MINUS, PLUS, RIGHT_BRACKET, RIGHT_PAREN, SEMICOLON, STAR, TILDE,
    CIRCUMFLEX, LEFT_CURLY, RIGHT_CURLY,

    // Two-char tokens
    GREATER_EQUAL, LESS_EQUAL, ASSIGN, LABEL_RANGE,

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
     * a single-char token type known to Oberon-07.
     */
    static TokenType typeFromChar(char chr);

    /**
     * @brief Returns the token type of the keyword that corresponds to a given lexeme.
     *
     * @attention internally this method works with case-sensitive comparisons and the input
     * lexeme must be all upper-case as this is how the Oberon language specifies its keywords.
     * Any desired support for lowercase (or mixed case) keywords must be provided by the
     * callers of this method. For example, the caller can (and should) convert the lexeme to
     * upper-case before calling this method. Any lexeme not recognized as keyword will be
     * considered an identifier - the caller must thus be sure that the lexeme doesn't match any
     * non-keyword token at the time it calls this method.
     *
     * @param lex the lexeme whose keyword token type should be returned.
     * @return the keyword token type corresponding to the lexeme.
     */
    static TokenType keywordTypeFromLexeme(const std::string& lex);


    /**
     * @brief Gets the token type of a given identifier lexeme.
     *
     * @param lowerCaseKeywords should assume lowercase keywords?
     * @param idLex the identifier lexeme whose token type should be determined.
     *
     * @return the token type of an identifier lexeme - the lexeme can be of a language keyword
     * or of an ordinary identifier.
     */
    static TokenType typeFromIdentifierLexeme(bool lowerCaseKeywords, const std::string& idLex);
};

std::ostream& operator<<(std::ostream&, const Token&);

#endif
