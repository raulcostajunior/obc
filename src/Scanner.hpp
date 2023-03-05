#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <string_view>
#include <vector>

#include "ErrorInfo.hpp"
#include "Token.hpp"


struct ScanResults {
    std::vector<Token> tokens;
    std::vector<ErrorInfo> errors;
};

struct ScanContext;

class Scanner {
   public:
    explicit Scanner(bool lowerCaseKeywords = false);

    /**
     * @brief Scans a given source file, returning the list of tokens found in it.
     *
     * @param srcFilePath the path of the source file to be scanned.
     *
     * @return list of tokens (and the lexical errors) in the file.
     */
    ScanResults scanSrcFile(const std::string& srcFilePath) const;

    /**
     * @brief Scans a string with the contents of a source file.
     *
     * @param src the contents of a source file.
     *
     * @return list of tokens (and the lexical errors) in the contents.
     */
    ScanResults scan(const std::string& src) const;

   private:
    bool m_lowerCaseKeywords;

    /**
     * @brief Scans the next token from the src input.
     *
     * Keeps advancing the src input until the next token (or an error) is found.
     *
     * @param ctx the context of the ongoing scan operation.
     */
    void scanToken(ScanContext& ctx) const;
};


#endif