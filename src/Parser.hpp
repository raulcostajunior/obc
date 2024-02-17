#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>

#include "Token.hpp"

class Parser {
   public:
    Parser(std::vector<Token> &&tokens);

   private:
    std::vector<Token> m_tokens;
};


#endif // PARSER_HPP
