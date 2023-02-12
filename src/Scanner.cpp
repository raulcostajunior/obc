#include "Scanner.hpp"

#include <array>
#include <fstream>

// Size of the buffer for storing a errno corresponding message.
const size_t ERR_MSG_BUFF_SIZE = 256;

Scanner::Scanner(bool upperCaseKeywords) : m_upperCaseKeywords(upperCaseKeywords) {}

ScanResults Scanner::scanSrcFile(const std::string& srcFilePath) {
    std::string src;
    { // Scope for the srcFile ifstream - allows its destruction before lexing work
      // actually takes place.
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
            // Reads the source file line by line and stores its contents in primary
            // memory.
            std::string nextLine;
            std::getline(srcFile, nextLine);
            src.append(nextLine);
        }
        if (srcFile.fail()) {
            // Some error happened during the file read operation.
            ScanResults res;
            std::array<char, ERR_MSG_BUFF_SIZE> errBuf{};
            strerror_s(errBuf.data(), errBuf.size(), errno);
            res.errors.emplace_back(
                  ErrorInfo{.msg = std::string{"Error while reading '" + srcFilePath +
                                               "': " + errBuf.data()}});
            return res;
        }
    }
    // Scans the source file from its in-memory storage.
    return scan(src);
}


ScanResults Scanner::scan(const std::string& src) {
    ScanResults res;
    return res;
}
