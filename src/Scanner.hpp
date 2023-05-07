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
    /**
     * @brief Scans a given source file, returning the list of tokens found in it.
     *
     * @param srcFilePath the path of the source file to be scanned.
     * @param lowerCaseKeywords adopt accept lowercase keywords?
     *
     * @return list of tokens (and the lexical errors) in the file.
     *
     * @note Lower case keywords mode has been introduced because of the high number of
     * opinions against all upper case keywords.
     */
    static ScanResults scanSrcFile(const std::string& srcFilePath,
                                   bool lowerCaseKeywords = false);

    /**
     * @brief Scans a string with the contents of a source file.
     *
     * @param src the contents of a source file.
     * @param lowerCaseKeywords adopt lowercase keywords?
     *
     * @return list of tokens (and the lexical errors) in the contents.
     *
     * @note Lower case keywords mode has been introduced because of the high number of
     * opinions against all upper case keywords.
     */
    static ScanResults scan(const std::string& src, bool lowerCaseKeywords = false);

   private:
    /**
     * @brief Scans the next token from the src input.
     *
     * Keeps advancing the src input until the next token (or an error) is found.
     *
     * @param ctx the context of the ongoing scan operation.
     */
    static void scanNextToken(ScanContext& ctx);

    /**
     * @brief Gets the token type of a given identifier lexeme.
     *
     * @param ctx the context of the ongoing scan operation.
     * @param idLex the identifier lexeme whose token type should be determined.
     *
     * @return the token type of an identifier lexeme - the lexeme can be of a language keyword
     * or of an ordinary identifier.
     */
    static TokenType tokenTypeFromIdentifierLexeme(const ScanContext& ctx,
                                                   const std::string& idLex);

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
     *
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
     * comment - after a * "(*", but before a "*)".
     *
     * @param ctx the context of the ongoing scan operation.
     */
    static void consumeComment(ScanContext& ctx);

    /**
     * @brief Consumes the scanning input until an end of string character (the double quotes)
     * is found.
     *
     * @attention this internal method must be called only when the scanner knows that is in a
     * string literal - after the initial double quotes, but before the final double quotes. A
     * string literal in Oberon must be on single line - an end of line before the string's
     * closing double quotes triggers an error.
     *
     * @param ctx the context of the ongoing scan operation.
     *
     */
    static void scanString(ScanContext& ctx);

    /**
     * @brief Scans an identifier - sequence of letters and digits initiated by a letter.
     *
     * An identifier can be a keyword of the language or can be a simple identifier (e.g. a
     * variable or constant name).
     *
     * @param ctx the context of the ongoing scan operation.
     * @param firstLetter the first letter of the identifier.
     */
    static void scanIdentifier(ScanContext& ctx, char firstLetter);

    /**
     * @brief Scans a number - sequence of digits optionally in hexadecimal form - or a single
     * char string - sequence of digits or hexadecimal digits followed by an "X". An hexadecimal
     * number literal must be followed by an "H" to be valid.
     *
     * @param ctx the context of the ongoing scan operation.
     * @param firstDigit the first digit of the number.
     */
    static void scanNumberOrSingleCharString(ScanContext& ctx, char firstDigit);

    /**
     * @brief Returns whether a given char is an hexadecimal digit or not.
     *
     * @note Oberon's grammar specifies only uppercase 'A' to 'F' as valid hexadecimal
     * digits. We stick to the specification in here.
     *
     * @param chr the character to be verified
     * @return true if chr is an hexadecimal digit; false otherwise.
     */
    static bool isHexDigit(char chr);
};


#endif