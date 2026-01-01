module;

#include <iostream>
#include <string>

export module token;

// The following 4 pre-processor definitions come from "minwindef.h" (part of WIN SDK) and are
// included when a toolchain based on the Microsoft C/C++ compiler is used (issue detected when
// MSVC 19.39.33523.0 was used, but most probably happens with other versions of the compiler
// as well). They have to be undefined before the declaration of the TokenType enum to prevent
// the pre-processor from turning them into invalid source.
#undef CONST
#undef TRUE
#undef FALSE
#undef IN

namespace obc {

    export enum class TokenType : unsigned char {
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

        // EOM (End-of-Module - sentinel value to ease the parser's life)
        EOM,

        // clang-format on
    };

    export struct Token {
        TokenType type;
        std::string lexeme;
        int line;

        std::string typeString() const;

        /**
         * @brief For single-char tokens, returns the token type for a given character.
         *
         * @param chr The character whose corresponding single-char token type should be
         * returned.
         * @return The single-char token type corresponding token type should be returned
         *
         * @throw invalid_argument exception if the given character does not correspond to
         * a single-char token type known to Oberon-07.
         */
        static TokenType typeFromChar(char chr);

        /**
         * @brief Returns the token type of the keyword that corresponds to a given lexeme.
         *
         * @note internally this method works with case-sensitive comparisons and the input
         * lexeme must be all upper-case as this is how the Oberon language specifies its
         * keywords. Any desired support for lowercase (or mixed case) keywords must be provided
         * by the callers of this method. For example, the caller can (and should) convert the
         * lexeme to upper-case before calling this method. Any lexeme not recognized as keyword
         * will be considered an identifier - the caller must thus be sure that the lexeme
         * doesn't match any non-keyword token at the time it calls this method.
         *
         * @param lex the lexeme whose keyword token type should be returned.
         * @return the keyword token type corresponding to the lexeme.
         */
        static TokenType keywordTypeFromLexeme(const std::string& lex);


        /**
         * @brief Gets the token type of given identifier lexeme.
         *
         * @param lowerCaseKeywords should assume lowercase keywords?
         * @param idLex the identifier lexeme whose token type should be determined.
         *
         * @return the token type of identifier lexeme - the lexeme can be of a language keyword
         * or of an ordinary identifier.
         */
        static TokenType typeFromIdentifierLexeme(bool lowerCaseKeywords,
                                                  const std::string& idLex);
    };

    export std::ostream& operator<<(std::ostream& out, const Token& token);

} // namespace obc
