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
     * @brief Scans a real number - sequence of digits in base 10 with a decimal separator. A
     * real number literal can have an optional scale factor at its end.
     *
     * @note: As the scanner can only know that it is handling a real number once it finds the
     * decimal separator, this method actually scans the decimal and the optional scale factor
     * at the end of the real number.
     *
     * @param ctx the context of the ongoing scan operation.
     * @param integerPart the integer part of the real number (includes the decimal separator).
     */
    static void scanRealNumber(ScanContext& ctx, const std::string& integerPart);

    /**
     * @brief Scans the optional scale factor at the end of a real number literal.
     *
     * @param ctx the context of the ongoing scan operation.
     * @param realBasePart the integer and decimal part (without any exponent) information of
     * the real number literal - the scanner can only know that the literal has a scale factor
     * when it finds the introducing 'E' of the exponential notation.
     */
    static void scanRealScaleFactor(ScanContext& ctx, const std::string& realBasePart);

    /**
     * Handles potential two-char tokens by looking ahead the next character in the source and
     * either consuming it as part of a two-char token if it matches the expected token or
     * returns immediately after registering the one-time token found.
     *
     * @param firstChr first character of the potentially two-character's token.
     * @param expectTokenType token type to be added if the second character matches what is
     * expected.
     * @param expectSecondChr expected look-ahead character needed to compose a two-character
     * token.
     * @param ctx the context of the ongoing scan operation.
     */
    static void handleTwoCharTokens(char firstChr, enum TokenType expectTokenType,
                                    char expectSecondChr, ScanContext& ctx);
};

#endif
