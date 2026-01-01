module;

#include <vector>

export module parser;

import token;

namespace obc {

    export class Parser {
       public:
        Parser(std::vector<Token> &&tokens);

       private:
        std::vector<Token> m_tokens;
    };

} // namespace obc
