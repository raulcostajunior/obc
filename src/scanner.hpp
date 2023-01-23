#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <vector>

#include "token.hpp"

class Scanner {

   public:
    Scanner(bool upperCaseKeywords=false);

    std::vector<Token>scanSrcFile(const std::string& srcFilePath);

   private:

    bool m_upperCaseKeywords;

};


#endif