module;

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

export module scanner:token;

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

    // Implementation for Token methods
    std::string Token::typeString() const {
        static std::map<TokenType, std::string> tokenTypeToString{

              {TokenType::ARRAY, "ARRAY"},
              {TokenType::BEGIN, "BEGIN"},
              {TokenType::BY, "BY"},
              {TokenType::CONST, "CONST"},
              {TokenType::DIV, "DIV"},
              {TokenType::DO, "DO"},
              {TokenType::ELSE, "ELSE"},
              {TokenType::ELSEIF, "ELSEIF"},
              {TokenType::END, "END"},
              {TokenType::IF, "IF"},
              {TokenType::MOD, "MOD"},
              {TokenType::MODULE, "MODULE"},
              {TokenType::OF, "OF"},
              {TokenType::OR, "OR"},
              {TokenType::PROCEDURE, "PROCEDURE"},
              {TokenType::RECORD, "RECORD"},
              {TokenType::THEN, "THEN"},
              {TokenType::TYPE, "TYPE"},
              {TokenType::VAR, "VAR"},
              {TokenType::WHILE, "WHILE"},
              {TokenType::IDENT, "IDENT"},
              {TokenType::INTEGER, "INTEGER"},
              {TokenType::STRING, "STRING"},
              {TokenType::REAL, "REAL"},

              {TokenType::AND, "AND"},
              {TokenType::COLON, "COLON"},
              {TokenType::COMMA, "COMMA"},
              {TokenType::DOT, "DOT"},
              {TokenType::EQUAL, "EQUAL"},
              {TokenType::GREATER, "GREATER"},
              {TokenType::HASH, "HASH"},
              {TokenType::LEFT_BRACKET, "LEFT_BRACKET"},
              {TokenType::LEFT_PAREN, "LEFT_PAREN"},
              {TokenType::LESS, "LESS"},
              {TokenType::MINUS, "MINUS"},
              {TokenType::PLUS, "PLUS"},
              {TokenType::RIGHT_BRACKET, "RIGHT_BRACKET"},
              {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
              {TokenType::SEMICOLON, "SEMICOLON"},
              {TokenType::STAR, "STAR"},
              {TokenType::TILDE, "TILDE"},

              {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
              {TokenType::LESS_EQUAL, "LESS_EQUAL"},
              {TokenType::ASSIGN, "ASSIGN"},
              {TokenType::EOM, "EOM"}

        };

        return tokenTypeToString[this->type];
    }

    TokenType Token::typeFromChar(const char chr) {
        static std::unordered_map<char, TokenType> charToSingleCharTokenType{
              {'&', TokenType::AND},           {':', TokenType::COLON},
              {',', TokenType::COMMA},         {'.', TokenType::DOT},
              {'=', TokenType::EQUAL},         {'>', TokenType::GREATER},
              {'#', TokenType::HASH},          {'[', TokenType::LEFT_BRACKET},
              {'(', TokenType::LEFT_PAREN},    {'<', TokenType::LESS},
              {'-', TokenType::MINUS},         {'+', TokenType::PLUS},
              {']', TokenType::RIGHT_BRACKET}, {')', TokenType::RIGHT_PAREN},
              {';', TokenType::SEMICOLON},     {'*', TokenType::STAR},
              {'~', TokenType::TILDE},         {'^', TokenType::CIRCUMFLEX},
              {'{', TokenType::LEFT_CURLY},    {'}', TokenType::RIGHT_CURLY}};
        const auto& iter = charToSingleCharTokenType.find(chr);
        if (iter == charToSingleCharTokenType.end()) {
            std::ostringstream oss{"Unexpected char, '"};
            oss << chr << "' found.";
            throw std::invalid_argument(oss.str());
        }
        return iter->second;
    }

    TokenType Token::keywordTypeFromLexeme(const std::string& lex) {
        static std::unordered_map<std::string, TokenType> lexToKeywordType{
              {"ARRAY", TokenType::ARRAY},     {"BEGIN", TokenType::BEGIN},
              {"CONST", TokenType::CONST},     {"DIV", TokenType::DIV},
              {"DO", TokenType::DO},           {"ELSE", TokenType::ELSE},
              {"ELSEIF", TokenType::ELSEIF},   {"END", TokenType::END},
              {"IF", TokenType::IF},           {"MOD", TokenType::MOD},
              {"MODULE", TokenType::MODULE},   {"OF", TokenType::OF},
              {"OR", TokenType::OR},           {"PROCEDURE", TokenType::PROCEDURE},
              {"RECORD", TokenType::RECORD},   {"THEN", TokenType::THEN},
              {"TYPE", TokenType::TYPE},       {"VAR", TokenType::VAR},
              {"WHILE", TokenType::WHILE},     {"TRUE", TokenType::TRUE},
              {"FALSE", TokenType::FALSE},     {"NIL", TokenType::NIL},
              {"FOR", TokenType::FOR},         {"RETURN", TokenType::RETURN},
              {"REPEAT", TokenType::REPEAT},   {"CASE", TokenType::CASE},
              {"UNTIL", TokenType::UNTIL},     {"IMPORT", TokenType::IMPORT},
              {"POINTER", TokenType::POINTER}, {"IN", TokenType::IN},
              {"IS", TokenType::IS},           {"TO", TokenType::TO}};
        const auto& iter = lexToKeywordType.find(lex);
        if (iter == lexToKeywordType.end()) {
            return TokenType::IDENT;
        }
        return iter->second;
    }

    TokenType Token::typeFromIdentifierLexeme(const bool lowerCaseKeywords,
                                              const std::string& idLex) {
        if (!lowerCaseKeywords) {
            // The scanner is supporting the standard casing of Oberon keywords: the lexeme
            // must be provided in all upper case to be recognized as a keyword.
            return keywordTypeFromLexeme(idLex);
        }
        // The scanner is in all-lowercase mode - if the lexeme is all lowercase, we pass its
        // uppercase form to the keyword matcher.
        std::string upperLex(idLex.size(), ' ');
        for (std::size_t i = 0; i < idLex.size(); i++) {
            if (idLex.at(i) == std::tolower(idLex.at(i))) {
                upperLex.at(i) = static_cast<char>(std::toupper(idLex.at(i)));
            } else {
                break;
            }
        }
        if (upperLex.size() == idLex.size()) {
            // A completed uppercase form means the lexeme is all lowercase
            return keywordTypeFromLexeme(upperLex);
        }
        // If this point is reached, it means that the lexeme was not all lowercase. There's no
        // chance for it to be a lowercase keyword; it is assumed to be an identifier.
        return TokenType::IDENT;
    }

    export std::ostream& operator<<(std::ostream& out, const Token& token);

    export std::ostream& operator<<(std::ostream& out, const Token& token) {
        out << "'" << token.lexeme << "' (" << token.typeString() << ") at line " << token.line;
        return out;
    }



} // namespace obc
