module;

#include <vector>

module parser;

import scanner;

namespace obc {

    Parser::Parser(std::vector<Token>&& tokens) : m_tokens(std::move(tokens)) {}

} // namespace obc
