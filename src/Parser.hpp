#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>

#include "Token.hpp"

namespace obc {

    class Parser {
       public:
        Parser(std::vector<Token> &&tokens);

       private:
        std::vector<Token> m_tokens;
    };

} // namespace obc

#endif // PARSER_HPP
