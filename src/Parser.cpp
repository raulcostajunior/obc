#include "Parser.hpp"

#include <utility>

Parser::Parser(std::vector<Token>&& tokens) : m_tokens(std::move(tokens)) {}
