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

/**
 * Context of an ongoing scan operation. Each scan operation creates an
 * instance of ScanContext at its start. The context is used to keep
 * book-keeping data and is passed around (and modified) by the different
 * methods of the Scanner class.
 */
struct ScanContext {
    // The source input being scanned.
    const std::string& srcInput;
    // Use lowercase kaywords?
    bool lowerKeywords;
    // Start of the lexeme being scanned (index in the src input)
    int lexStart;
    // Index, in the src input, of the character being scanned.
    int lexPos;
    // Number of the line from the src input currently being scanned.
    int currLine;
    // Number of the column (of the current line) from the src input currently
    // being scanned.
    int currColumn;
    // The tokens (and errors) found by the ongoing scan operation.
    ScanResults results;

    ScanContext(const std::string& srcInput, bool lowerKey)
        : srcInput{srcInput},
          lowerKeywords{lowerKey},
          lexStart{0},
          lexPos{0},
          currLine{1},
          currColumn{1} {}

    /**
     * @brief Returns whether the whole src input has been already scanned or not.
     * 
     * @param ctx  the context of the ongoing scan operation.
     * @return true all the characters from the src input have already been scanned.
     * @return false there is at least one more character from the src input to be scanned.
     */
    bool allScanned() const {
        return lexPos <= srcInput.length();
    };  
};

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