#include "Token.hpp"

#include <map>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

std::string Token::typeString() const {
    static std::map<TokenType, std::string> tokenTypeToString{

          {TokenType::ARRAY, "ARRAY"},
          {TokenType::BEGIN, "BEGIN"},
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
          {TokenType::STRING, "STRING"},
          {TokenType::NUMBER, "NUMBER"},

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
          {TokenType::ASSIGN, "ASSIGN"}

    };

    return tokenTypeToString[this->type];
}

TokenType Token::typeFromChar(const char ch) {
    static std::unordered_map<char, TokenType> charToSingleCharTokenType{
          {'&', TokenType::AND},           {':', TokenType::COLON},
          {',', TokenType::COMMA},         {'.', TokenType::DOT},
          {'=', TokenType::EQUAL},         {'>', TokenType::GREATER},
          {'#', TokenType::HASH},          {'[', TokenType::LEFT_BRACKET},
          {'(', TokenType::LEFT_PAREN},    {'<', TokenType::LESS},
          {'-', TokenType::MINUS},         {'+', TokenType::PLUS},
          {']', TokenType::RIGHT_BRACKET}, {')', TokenType::RIGHT_PAREN},
          {';', TokenType::SEMICOLON},     {'*', TokenType::STAR},
          {'~', TokenType::TILDE}};
    const auto& it = charToSingleCharTokenType.find(ch);
    if (it == charToSingleCharTokenType.end()) {
        std::ostringstream oss{"Unexpected char, '"};
        oss << ch << "' found.";
        throw std::invalid_argument(oss.str());
    }
    return it->second;
}

TokenType Token::keywordTypeFromLexeme(const std::string& lex) {
    static std::unordered_map<std::string, TokenType> lexToKeywordType{
          {"ARRAY", TokenType::ARRAY},   {"BEGIN", TokenType::BEGIN},
          {"CONST", TokenType::CONST},   {"DIV", TokenType::DIV},
          {"DO", TokenType::DO},         {"ELSE", TokenType::ELSE},
          {"ELSEIF", TokenType::ELSEIF}, {"END", TokenType::END},
          {"IF", TokenType::IF},         {"MOD", TokenType::MOD},
          {"MODULE", TokenType::MODULE}, {"OF", TokenType::OF},
          {"OR", TokenType::OR},         {"PROCEDURE", TokenType::PROCEDURE},
          {"RECORD", TokenType::RECORD}, {"THEN", TokenType::THEN},
          {"TYPE", TokenType::TYPE},     {"VAR", TokenType::VAR},
          {"WHILE", TokenType::WHILE},
    };
    const auto& it = lexToKeywordType.find(lex);
    if (it == lexToKeywordType.end()) {
        std::ostringstream oss{"Provided string, '"};
        oss << lex << "' does not match any keyword.";
        throw std::invalid_argument(oss.str());
    }
    return it->second;
}

std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << "'" << token.lexeme << "' (" << token.typeString() << ")";
    return out;
}
