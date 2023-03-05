#include "Scanner.hpp"

#include <array>
#include <cstring>
#include <fstream>
#include <string_view>

// Size of the buffer for storing an errno corresponding message.
const size_t ERR_MSG_BUFF_SIZE = 256U;

/**
 * Context of an ongoing scan operation. Each scan operation creates an
 * instance of ScanContext at its start. The context stores book-keeping data for the
 * scan process and is passed around (and modified) by the different methods of the
 * Scanner class.
 */
struct ScanContext {
    // The source input being scanned.
    // A string_view here is safe - the lifetime of a ScanContext is limited to
    // a single scan operation. The string_view allows avoiding both copying strings and
    // having to declare a const data member.
    std::string_view srcInput;
    // Use lowercase keyword?
    bool lowerKeywords;
    // Start of the lexeme being scanned (index in the src input)
    int lexStart{0};
    // Index, in the src input, of the character being scanned.
    int lexPos{0};
    // Number of the line from the src input currently being scanned.
    int currLine{1};
    // Number of the column (of the current line) from the src input currently
    // being scanned.
    int currColumn{1};
    // The tokens (and errors) found by the ongoing scan operation.
    ScanResults results;

    ScanContext(const std::string& srcInput, bool lowerKey)
        : srcInput{srcInput}, lowerKeywords{lowerKey} {}

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

Scanner::Scanner(bool lowerCaseKeywords) : m_lowerCaseKeywords(lowerCaseKeywords) {}

ScanResults Scanner::scanSrcFile(const std::string& srcFilePath) const {
    std::string src;
    { // Scope for the srcFile ifstream - allows its destruction before lexing
      // work actually takes place.
        std::ifstream srcFile(srcFilePath);
        if (!srcFile.is_open()) {
            // Some error happened during file opening.
            ScanResults res;
            res.errors.emplace_back(
                  ErrorInfo{.msg = std::string{"File '"} + srcFilePath +
                                   "' not found or not available for reading."});
            return res;
        }
        while (srcFile) {
            // Reads the source file line by line and stores its contents in
            // primary memory.
            std::string nextLine;
            std::getline(srcFile, nextLine);
            src.append(nextLine);
            // As std::getline consumes the delimiter - in this case the default
            // "\n" - we add it to the string. We may end up adding an extra
            // "\n" when the last line in the source file didn't end with a "\n"
            // - for the context of the scanner, though, this extra line break
            // is harmless as the new last line in memory will have no code to
            // be scanned.
            src.append("\n");
        }
        srcFile.close();
        if (srcFile.bad()) {
            // Some error happened during the file read operation.
            // NOTE: fail() should not be used for detecting read errors in this
            //       context: a file whose last line consists solely of the
            //       new line character would cause getline to return no
            //       character and set both eofbit and failbit.
            ScanResults res;
            std::array<char, ERR_MSG_BUFF_SIZE> errBuf{};
#if defined(__MSVCRT__) || defined(_MSC_VER)
            // On Windows, using the Microsoft supplied runtime, the safe
            // version of strerror is not strerror_r, but strerror_s.
            strerror_s(errBuf.data(), errBuf.size(), errno);
#else
            strerror_r(errno, errBuf.data(), errBuf.size());
#endif
            res.errors.emplace_back(
                  ErrorInfo{.msg = std::string{"Error while reading '" + srcFilePath +
                                               "': " + errBuf.data()}});
            return res;
        }
    }
    // Scans the source file from its in-memory storage.
    return scan(src);
}


ScanResults Scanner::scan(const std::string& src) const {
    ScanContext ctx(src, m_lowerCaseKeywords);

    while (ctx.allScanned()) {
        scanToken(ctx);
    }

    return ctx.results;
}

void Scanner::scanToken(ScanContext& ctx) const {
    // TODO: add definition.
}
