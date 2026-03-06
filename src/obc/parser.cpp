module;

#include <vector>

module obc.parser;

import obc.scanner;

namespace obc {

    Parser::Parser(std::vector<Token>&& tokens) : m_tokens(std::move(tokens)) {}

} // namespace obc
