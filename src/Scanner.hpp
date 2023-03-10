#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <string_view>
#include <utility>
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
    void scanNextToken(ScanContext& ctx) const;

    /**
     * @brief Gets the token type of a given identifier lexeme.
     *
     * @param idLex the identifier lexeme whose token type should be determined.
     *
     * @return the token type of an identifier lexeme - the lexeme can be of a language keyword
     * or of an ordinary identifier.
     */
    TokenType tokenTypeFromIdentifierLexeme(const std::string& idLex) const;

    /**
     * @brief Returns whether the whole src input has been already scanned or not.
     *
     * @param ctx  the context of the ongoing scan operation.
     * @return true all the characters from the src input have already been scanned.
     * @return false there is at least one more character from the src input to be scanned.
     */
    static bool allScanned(const ScanContext& ctx);

    /**
     * @brief Returns the next character in the source being scanned and advances the scan by
     * one character.
     * @param ctx the context of the ongoing scan operation.
     * @return the next character in the source being scanned.
     */
    static char nextChr(ScanContext& ctx);

    /**
     * @brief Returns the next character in the source being scanned, but doesn't advance the
     * scan.
     * @param ctx the context of the ongoing scan operation.
     * @return the next character in the source being scanned. If the end of the input has been
     * reached, returns '\0'.
     */
    static char nextChrNoAdvance(const ScanContext& ctx);

    /**
     * @brief Returns whether the next character matches an expected character or not.
     *
     * If the next character matches the expected character it is also "consumed", by advancing
     * the scan by one character.
     *
     * @param ctx the context of the ongoing scan operation.
     * @param expChr the expected character after the current one.
     * @return true if the next character matches the expected character.
     */
    static bool nextChrMatch(ScanContext& ctx, char expChr);

    /**
     * @brief Consumes the scanning input until an end of comment sequence, "*)", is found.
     *
     * @attention this internal method must be called only when the scanner knows that is in a
     * comment - after a
     * "(*", but before a "*)".
     *
     * @param ctx the context of the ongoing scan operation.
     */
    static void consumeComment(ScanContext& ctx);

    static std::string getIdentifierLexeme(ScanContext& ctx);

    static std::string getNumberLexeme(ScanContext& ctx);
};


#endif