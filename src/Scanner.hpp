#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <vector>

#include "ErrorInfo.hpp"
#include "Token.hpp"


struct ScanResults {
    std::vector<Token> tokens;
    std::vector<ErrorInfo> errors;
};

class Scanner {
   public:
    Scanner(bool upperCaseKeywords = false);

    /**
     * Scans a given source file, returning the list of tokens found in it.
     * @param srcFilePath the path of the source file to be scanned.
     * @return list of tokens in the file.
     */
    ScanResults scanSrcFile(const std::string& srcFilePath);

    /**
     * Scan an a string with the contents of a source file.
     * @param src the contents of a source file.
     * @return list of tokens in the contents.
     */
    ScanResults scan(const std::string& src);

   private:
    bool m_upperCaseKeywords;
};


#endif