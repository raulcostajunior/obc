module;

#include <vector>

export module obc.parser;

import obc.scanner;

namespace obc {

    export class Parser {
       public:
        Parser(std::vector<Token> &&tokens);

       private:
        std::vector<Token> m_tokens;
    };

} // namespace obc
