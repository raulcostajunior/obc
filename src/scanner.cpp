module;

#include <array>
#include <fstream>
#include <string_view>

module scanner;

import token_utils;
import error_info;

namespace obc {
    // Size of the buffer for storing an errno corresponding message.
    constexpr size_t ERR_MSG_BUFF_SIZE = 256U;

    /**
     * Context of an ongoing scan operation. Each scan operation creates an
     * instance of ScanContext at its start. The context stores bookkeeping data for the
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
        bool lowerCaseKeywords;
        // Should currColumn be ignored? (Look in currColumn description for more details)
        bool ignoreCurrColumn;
        // Index, in the src input, of the character being scanned.
        unsigned long lexPos{0};
        // Number of the line from the src input currently being scanned.
        int currLine{1};
        // Number of the column (of the current line) from the src input currently
        // being scanned. Column information should be ignored if there's at least one '\t' in
        // the source file.
        int currColumn{1};
        // The tokens (and errors) found by the ongoing scan operation.
        ScanResults results;

        ScanContext(const std::string& srcInput, const bool lowerKey,
                    const bool ignoreCurrColumn)
            : srcInput{srcInput},
              lowerCaseKeywords{lowerKey},
              ignoreCurrColumn{ignoreCurrColumn} {}

        int getCurrColumn() const {
            if (ignoreCurrColumn) {
                return -1;
            }
            return currColumn;
        }
    };

    ScanResults Scanner::scanSrcFile(const std::string& srcFilePath, bool lowerCaseKeywords) {
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
                // "\n" - we add it to the string if the end of the file hasn't still been
                // reached.
                if (srcFile) {
                    src.append("\n");
                }
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
        return scan(src, lowerCaseKeywords);
    }


    ScanResults Scanner::scan(const std::string& src, const bool lowerCaseKeywords) {
        // Current column information should be ignored when the source file has at least one
        // tab: The information of how many columns correspond to a '\t' is not in the source
        // file and cannot be easily inferred.
        const bool srcHasTab = src.find('\t') != std::string::npos;
        ScanContext ctx(src, lowerCaseKeywords, srcHasTab);

        while (!allScanned(ctx)) {
            scanNextToken(ctx);
        }

        // An End-of-Module is always inserted to provide a clear indicator for the parser.
        ctx.results.tokens.emplace_back(
              Token{.type = TokenType::EOM, .lexeme = "", .line = ctx.currLine});

        return ctx.results;
    }

    bool Scanner::allScanned(const ScanContext& ctx) {
        return ctx.lexPos >= ctx.srcInput.length();
    }

    char Scanner::nextChr(ScanContext& ctx) {
        const char chr = ctx.srcInput[ctx.lexPos];
        ctx.lexPos++;
        return chr;
    }

    char Scanner::nextChrNoAdvance(const ScanContext& ctx) {
        if (allScanned(ctx)) {
            return '\0';
        }
        return ctx.srcInput[ctx.lexPos];
    }

    bool Scanner::nextChrMatch(ScanContext& ctx, const char expChr) {
        if (allScanned(ctx) || ctx.srcInput[ctx.lexPos] != expChr) {
            return false;
        }
        ctx.lexPos++;
        return true;
    }

    void Scanner::scanNextToken(ScanContext& ctx) {
        switch (const char chr = nextChr(ctx)) {
            // Handling of single-char tokens
            case '&':
            case ',':
            case '=':
            case '#':
            case '[':
            case '-':
            case '+':
            case ']':
            case ')':
                // A close parenthesis matched in this context won't be ont of the comments
                // terminating characters. Such right parenthesis will be consumed by th comment
                // consuming loop.
            case ';':
            case '*':
                // A star matched in this context won't be one of the comments terminating
                // characters. Such stars will be consumed by the comment consuming loop.
            case '~':
            case '{':
            case '}':
            case '^':
                try {
                    ctx.results.tokens.emplace_back(Token{.type = Token::typeFromChar(chr),
                                                          .lexeme = std::string{chr},
                                                          .line = ctx.currLine});
                } catch (std::invalid_argument const& ex) {
                    ctx.results.errors.emplace_back(ErrorInfo{.line = ctx.currLine,
                                                              .column = ctx.getCurrColumn(),
                                                              .msg = ex.what()});
                }
                ctx.currColumn++;
                break;

            // Handling of (potentially) two-char tokens
            case '<':
                handleTwoCharTokens(chr, TokenType::LESS_EQUAL, '=', ctx);
                break;
            case '>':
                handleTwoCharTokens(chr, TokenType::GREATER_EQUAL, '=', ctx);
                break;
            case ':':
                handleTwoCharTokens(chr, TokenType::ASSIGN, '=', ctx);
                break;
            case '.':
                handleTwoCharTokens(chr, TokenType::LABEL_RANGE, '.', ctx);
                break;

            // Handling of whitespace characters (except newline) - simply consumed. Blanks are
            // not ignored when inside strings.
            case ' ':
            case '\r':
            case '\t':
                ctx.currColumn++;
                break;

            // Handling of new lines (outside comments; new lines in the middle of comments are
            // handled by the comment handler)
            case '\n':
                ctx.currLine++;
                ctx.currColumn = 1;
                break;

            // Handling of (potential) comments. If the "(" is followed by a "*" and indeed
            // starts a comment, the scanning process will be captured by the comment-consuming
            // loop.
            case '(':
                if (nextChrMatch(ctx, '*')) {
                    // Found start of comment - "consume" it.
                    ctx.currColumn++;
                    consumeComment(ctx);
                    break;
                } // Found a single-character open parenthesis token.
                ctx.results.tokens.emplace_back(Token{.type = Token::typeFromChar(chr),
                                                      .lexeme = std::string{chr},
                                                      .line = ctx.currLine});
                ctx.currColumn++;
                break;

            // Handling of string literals. String literals cannot contain internal double
            // quotes and cannot span across multiple lines.
            case '"':
                ctx.currColumn++;
                scanString(ctx);
                break;

            default:
                ctx.currColumn++;
                if (std::isalpha(chr) != 0) {
                    scanIdentifier(ctx, chr);
                } else if (std::isdigit(chr) != 0) {
                    scanNumberOrSingleCharString(ctx, chr);
                } else {
                    ctx.results.errors.emplace_back(ErrorInfo{
                          .line = ctx.currLine,
                          .column = ctx.getCurrColumn(),
                          .msg = std::string{"Unexpected character, '"} + chr + "' found."});
                }
        }
    }

    void Scanner::scanNumberOrSingleCharString(ScanContext& ctx, const char firstDigit) {
        std::string lex{firstDigit};
        char nextChr = nextChrNoAdvance(ctx);
        while (isHexDigit(nextChr)) {
            lex.push_back(nextChr);
            ctx.lexPos++;
            ctx.currColumn++;
            nextChr = nextChrNoAdvance(ctx);
        }
        if (nextChr == 'X') {
            // The end of a single character string has been found. The character must be
            // evaluated from the hexadecimal value given by the lexeme.
            if (lex.size() > 2) {
                ctx.results.errors.push_back(ErrorInfo{
                      .line = ctx.currLine,
                      .column = ctx.getCurrColumn(),
                      .msg = "Single character strings must have values between 0 and FF."});
            } else {
                const int charCode = std::stoi(
                      lex, nullptr,
                      16); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                ctx.results.tokens.emplace_back(
                      Token{.type = TokenType::STRING,
                            .lexeme = std::string{static_cast<char>(charCode)},
                            .line = ctx.currLine});
            }
            // Consume the 'X' - it is not part of the string
            ctx.lexPos++;
            ctx.currColumn++;
        } else if (nextChr == 'H') {
            // The end of an integer literal in hexadecimal form has been found - the H is part
            // of the integer literal and must be included in its lexeme.
            lex.push_back(nextChr);
            ctx.lexPos++;
            ctx.currColumn++;
            ctx.results.tokens.emplace_back(
                  Token{.type = TokenType::INTEGER, .lexeme = lex, .line = ctx.currLine});
        } else if (nextChr == '.') {
            // A decimal separator indicates that a REAL literal is being scanned.
            if (!allBase10Digits(lex)) {
                // Oberon only allows integer numbers to be represented in hex. Real numbers
                // must be always expressed in base 10.
                ctx.results.errors.emplace_back(
                      ErrorInfo{.line = ctx.currLine,
                                .column = ctx.getCurrColumn(),
                                .msg = "Real numbers must use only digits between 0 and 9."});
            }
            lex.push_back(nextChr);
            ctx.lexPos++;
            ctx.currColumn++;
            scanRealNumber(ctx, lex);
        } else {
            // The lexeme found so far can be an integer literal in decimal form - unless it
            // contains any hexadecimal digit that is not a base 10 digit.
            if (allBase10Digits(lex)) {
                // The lexeme is a valid integer literal in decimal form.
                ctx.results.tokens.emplace_back(
                      Token{.type = TokenType::INTEGER, .lexeme = lex, .line = ctx.currLine});
            } else {
                // A hexadecimal digit that is not a base 10 digit has been found; report the
                // error.
                ctx.results.errors.emplace_back(
                      ErrorInfo{.line = ctx.currLine,
                                .column = ctx.getCurrColumn(),
                                .msg = "Hexadecimal number must be terminated with an 'H'."});
            }
        }
    }

    void Scanner::scanRealNumber(ScanContext& ctx, const std::string& integerPart) {
        std::string lex{integerPart};
        char nextChr = nextChrNoAdvance(ctx);
        while (isdigit(nextChr) != 0) {
            lex.push_back(nextChr);
            ctx.lexPos++;
            ctx.currColumn++;
            nextChr = nextChrNoAdvance(ctx);
        }
        if (nextChr == 'E') {
            // Found the optional scale factor at the end.
            lex.push_back(nextChr);
            ctx.lexPos++;
            ctx.currColumn++;
            scanRealScaleFactor(ctx, lex);
        } else {
            ctx.results.tokens.emplace_back(
                  Token{.type = TokenType::REAL, .lexeme = lex, .line = ctx.currLine});
        }
    }

    void Scanner::scanRealScaleFactor(ScanContext& ctx, const std::string& realBasePart) {
        std::string lex{realBasePart};
        char nextCh = nextChr(ctx);
        ctx.currColumn++;
        if (nextCh != '+' && nextCh != '-') {
            ctx.results.errors.push_back(ErrorInfo{
                  .line = ctx.currLine,
                  .column = ctx.getCurrColumn(),
                  .msg = "Real number scale factor must start with an 'E' followed by "
                         "either a '+' or '-' signal."});
        } else {
            lex.push_back(nextCh);
            nextCh = nextChr(ctx);
            ctx.currColumn++;
            if (isdigit(nextCh) == 0) {
                ctx.results.errors.push_back(ErrorInfo{
                      .line = ctx.currLine,
                      .column = ctx.getCurrColumn(),
                      .msg = "Scale factor of a real number must have at least one digit "
                             "after the '+' or '-' signal."});
            } else {
                lex.push_back(nextCh);
                nextCh = nextChrNoAdvance(ctx);
                while (isdigit(nextCh) != 0) {
                    lex.push_back(nextCh);
                    ctx.lexPos++;
                    ctx.currColumn++;
                    nextCh = nextChrNoAdvance(ctx);
                }
                ctx.results.tokens.push_back(
                      Token{.type = TokenType::REAL, .lexeme = lex, .line = ctx.currLine});
            }
        }
    }

    void Scanner::scanIdentifier(ScanContext& ctx, const char firstLetter) {
        std::string identLex{firstLetter};
        char nextChr = nextChrNoAdvance(ctx);
        while (isalpha(nextChr) != 0 || isdigit(nextChr) != 0) {
            identLex.push_back(nextChr);
            ctx.lexPos++;
            ctx.currColumn++;
            nextChr = nextChrNoAdvance(ctx);
        }
        const TokenType tkType =
              Token::typeFromIdentifierLexeme(ctx.lowerCaseKeywords, identLex);
        ctx.results.tokens.emplace_back(
              Token{.type = tkType, .lexeme = identLex, .line = ctx.currLine});
    }

    void Scanner::consumeComment(ScanContext& ctx) {
        bool endOfCommentFound = false;
        while (!allScanned(ctx)) {
            // As comments can be "surrounded" by real code (in Oberon-07, comments are not
            // ended by line breaks), the line and column information must be updated.
            if (nextChrNoAdvance(ctx) == '\n') {
                ctx.currLine++;
                ctx.currColumn = 1;
            } else {
                ctx.currColumn++;
            }
            if (nextChrNoAdvance(ctx) == '*') {
                // There's a chance that the end of comment has been reached;
                // Advances the scan and checks if the next character is ")"
                ctx.lexPos++;
                if (nextChrNoAdvance(ctx) == ')') {
                    // The end of the comment has indeed been reached.
                    ctx.lexPos++;
                    ctx.currColumn++;
                    endOfCommentFound = true;
                    break; // Break-out of the comment consuming loop
                }
            }
            ctx.lexPos++;
        }
        if (!endOfCommentFound) {
            // If the end of the comment has not been found at this point, it means we
            // have an unfinished comment.
            ctx.results.errors.emplace_back(
                  ErrorInfo{.line = ctx.currLine,
                            .column = ctx.getCurrColumn(),
                            .msg = "Source module ends in an unfinished comment."});
        }
    }

    void Scanner::scanString(ScanContext& ctx) {
        std::string strLex{};
        while (!allScanned(ctx)) {
            if (const char nextChr = nextChrNoAdvance(ctx); nextChr != '\n' && nextChr != '"') {
                // In the middle of the string literal - just keep on acquiring the lexeme
                strLex.push_back(nextChrNoAdvance(ctx));
                ctx.lexPos++;
                ctx.currColumn++;
            } else {
                if (nextChr == '\n') {
                    ctx.results.errors.emplace_back(ErrorInfo{
                          .line = ctx.currLine,
                          .column = ctx.getCurrColumn(),
                          .msg = "Unterminated string - strings must be on a single line."});
                } else {
                    // Double-quotes (End of string literal) found
                    ctx.lexPos++;
                    ctx.currColumn++;
                    ctx.results.tokens.emplace_back(Token{
                          .type = TokenType::STRING, .lexeme = strLex, .line = ctx.currLine});
                }
                break;
            }
        }
    }

    void Scanner::handleTwoCharTokens(const char firstChr, const TokenType expectTokenType,
                                      const char expectSecondChr, ScanContext& ctx) {
        std::string twoChrLex{firstChr};
        if (nextChrMatch(ctx, expectSecondChr)) {
            twoChrLex += expectSecondChr;
            ctx.results.tokens.emplace_back(
                  Token{.type = expectTokenType, .lexeme = twoChrLex, .line = ctx.currLine});
            ctx.currColumn += 2;
        } else {
            ctx.results.tokens.emplace_back(Token{.type = Token::typeFromChar(firstChr),
                                                  .lexeme = std::string{firstChr},
                                                  .line = ctx.currLine});
            ctx.currColumn++;
        }
    }

} // namespace obc
