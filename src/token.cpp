#include "token.hpp"

std::ostream& operator<<(std::ostream& ostr, const Token& token) {
    ostr << token.lexeme;
    return ostr;
}
