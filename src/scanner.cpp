#include "scanner.hpp"

Scanner::Scanner(bool upperCaseKeywords)
    : m_upperCaseKeywords(upperCaseKeywords) {}

std::vector<Token> Scanner::scanSrcFile(std::string srcFilePath) {
    return std::vector<Token>();
}
