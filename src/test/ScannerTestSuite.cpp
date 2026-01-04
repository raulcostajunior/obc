#include <gtest/gtest.h>

#include <filesystem>

import scanner;

using namespace obc;

TEST(ScannerTests, TestEmptyFile) { // NOLINT(*-throwing-static-initialization, *-owning-memory)
    // An empty file must have the EOM token and no errors.
    const auto [tokens, errors] = Scanner::scan("");
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens.at(tokens.size() - 1).type, TokenType::EOM);
    EXPECT_EQ(errors.size(), 0);
}

TEST(ScannerTests, TestLowerCaseKeywords) { // NOLINT(*-throwing-static-initialization, *-owning-memory, *-function-cognitive-complexity)
    const std::string lowerCaseSrc{
          R"(
module LowerCaseModule;

procedure WriteCounters;
    var i: integer;
    i := 0;
    while i < 4 do
      WriteInt(i);
      i := i + 1;
    end
end WriteCounters;

begin
  WriteCounters;
end LowerCaseModule.
)"};

    const std::string upperCaseSrc{
          R"(
MODULE UpperCaseModule;

PROCEDURE WriteCounters;
    VAR i: integer;
    i := 0;
    WHILE i < 4 DO
      WriteInt(i);
      i := i + 1;
    END
END WriteCounters;

BEGIN
  WriteCounters;
END LowerCaseModule.
)"};

    constexpr int expectTokens =
          42; // Expected number of tokens in src files used in this test.

    // A lexically valid file with lowerCase keywords should be successfully parsed with all
    // the keywords identified with the scanner in lowerCaseKeywords mode.
    auto res = Scanner::scan(lowerCaseSrc, true);
    ASSERT_EQ(res.errors.size(), 0);
    ASSERT_EQ(res.tokens.size(), expectTokens);
    EXPECT_EQ(res.tokens.at(0).type, TokenType::MODULE);
    EXPECT_EQ(res.tokens.at(1).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(38).type, TokenType::END);
    EXPECT_EQ(res.tokens.at(39).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(39).lexeme, "LowerCaseModule");
    EXPECT_EQ(res.tokens.at(40).type, TokenType::DOT);
    EXPECT_EQ(res.tokens.at(res.tokens.size() - 1).type, TokenType::EOM);

    // A lexically valid file with lowerCaseKeywords should be successfully parsed with none
    // of the keywords identified when the scanner is in the default uppercase-keywords mode.
    // All keywords should be identified as token identifiers.
    res = Scanner::scan(lowerCaseSrc);
    ASSERT_EQ(res.errors.size(), 0);
    ASSERT_EQ(res.tokens.size(), expectTokens);
    EXPECT_EQ(res.tokens.at(0).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(0).lexeme, "module");
    EXPECT_EQ(res.tokens.at(1).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(38).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(38).lexeme, "end");
    EXPECT_EQ(res.tokens.at(39).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(39).lexeme, "LowerCaseModule");
    EXPECT_EQ(res.tokens.at(40).type, TokenType::DOT);
    EXPECT_EQ(res.tokens.at(41).type, TokenType::EOM);

    // A lexically valid file with uppercase keywords should be successfully parsed with all
    // the keywords identified with the scanner in the default uppercase-keywords mode.
    res = Scanner::scan(upperCaseSrc);
    ASSERT_EQ(res.errors.size(), 0);
    ASSERT_EQ(res.tokens.size(), expectTokens);
    EXPECT_EQ(res.tokens.at(0).type, TokenType::MODULE);
    EXPECT_EQ(res.tokens.at(1).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(38).type, TokenType::END);
    EXPECT_EQ(res.tokens.at(39).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(39).lexeme, "LowerCaseModule");
    EXPECT_EQ(res.tokens.at(40).type, TokenType::DOT);
    EXPECT_EQ(res.tokens.at(41).type, TokenType::EOM);

    // A lexically valid file with uppercase keywords should be successfully parsed with none
    // of the keywords identified when the scanner is in lowercase-keywords mode.
    // All keywords should be identified as token identifiers.
    res = Scanner::scan(upperCaseSrc, true);
    ASSERT_EQ(res.errors.size(), 0);
    ASSERT_EQ(res.tokens.size(), expectTokens);
    EXPECT_EQ(res.tokens.at(0).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(0).lexeme, "MODULE");
    EXPECT_EQ(res.tokens.at(1).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(38).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(38).lexeme, "END");
    EXPECT_EQ(res.tokens.at(39).type, TokenType::IDENT);
    EXPECT_EQ(res.tokens.at(39).lexeme, "LowerCaseModule");
    EXPECT_EQ(res.tokens.at(40).type, TokenType::DOT);
    EXPECT_EQ(res.tokens.at(41).type, TokenType::EOM);
}

TEST(ScannerTests, TestModuleWithUnfinishedComment) { // NOLINT(*-throwing-static-initialization, *-owning-memory, *-function-cognitive-complexity)
    // A source file that finishes with an unfinished comment must trigger an error.
    const std::string unfinishedCommentMsg{"Source module ends in an unfinished comment."};

    const std::string moduleSrc{R"(
MODULE UnfinishedComment;

(* This is a multiline, open ended
comment and should be rejected.
)"};
    auto [tokens, errors] = Scanner::scan(moduleSrc);
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens.at(0).type, TokenType::MODULE);
    EXPECT_EQ(tokens.at(1).type, TokenType::IDENT);
    EXPECT_EQ(tokens.at(1).lexeme, "UnfinishedComment");
    EXPECT_EQ(tokens.at(2).type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens.at(tokens.size() - 1).type, TokenType::EOM);

    ASSERT_EQ(errors.size(), 1);
    EXPECT_EQ(errors.at(0).line, 6);
    EXPECT_EQ(errors.at(0).column, 1);
    EXPECT_EQ(errors.at(0).msg, unfinishedCommentMsg);
}

TEST(ScannerTests, TestModuleWithInvalidSymbol) { // NOLINT(*-throwing-static-initialization, *-owning-memory, *-function-cognitive-complexity)
    // A source file with an invalid terminal symbol must trigger a lexical error.
    // The error must not stop the scanner, which must continue finding tokens
    // until the end of the module.
    const std::string invalidSymbolSrc{R"(
MODULE WithInvalidSymbol;

(* ? is not a valid terminal in the language; it should be accepted in a 
comment, but trigger an error when outside a comment *)

BEGIN
    VAR i: INTEGER?;
    WriteInt(i);
END.
)"};

    auto [tokens, errors] = Scanner::scan(invalidSymbolSrc);
    ASSERT_EQ(tokens.size(), 17);
    EXPECT_EQ(tokens.at(0).type, TokenType::MODULE);
    EXPECT_EQ(tokens.at(1).type, TokenType::IDENT);
    EXPECT_EQ(tokens.at(1).lexeme, "WithInvalidSymbol");
    // Tokens 7 and 8 are the ones around the invalid symbol in the source.
    // We verify if they have been properly scanned.
    EXPECT_EQ(tokens.at(7).type, TokenType::IDENT);
    EXPECT_EQ(tokens.at(7).lexeme, "INTEGER");
    EXPECT_EQ(tokens.at(8).type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens.at(15).type, TokenType::DOT);
    EXPECT_EQ(tokens.at(tokens.size() - 1).type, TokenType::EOM);

    ASSERT_EQ(errors.size(), 1);
    // The R-String for the source starts with a new line, so the MODULE line
    // is already line 2 in the source - that's the reason for the line with the
    // invalid symbol being line 8.
    EXPECT_EQ(errors.at(0).line, 8);
    EXPECT_EQ(errors.at(0).msg, std::string{"Unexpected character, '?' found."});
}

TEST(ScannerTests, TestModuleWithStringLiteral) { // NOLINT(*-throwing-static-initialization, *-owning-memory, *-function-cognitive-complexity)
    namespace fs = std::filesystem;
    const std::string src_file_path{
          fs::path(__FILE__).parent_path().append("oberon_src").append("Hello.Mod").string()};
    auto [tokens, errors] = Scanner::scanSrcFile(src_file_path);
    ASSERT_EQ(tokens.size(), 18);
    EXPECT_EQ(tokens.at(0).type, TokenType::MODULE);
    EXPECT_EQ(tokens.at(1).type, TokenType::IDENT);
    EXPECT_EQ(tokens.at(1).lexeme, "Hello");
    EXPECT_EQ(tokens.at(3).type, TokenType::BEGIN);
    EXPECT_EQ(tokens.at(4).type, TokenType::IDENT);
    EXPECT_EQ(tokens.at(4).lexeme, "WriteLn");
    EXPECT_EQ(tokens.at(5).type, TokenType::LEFT_PAREN);
    EXPECT_EQ(tokens.at(6).type, TokenType::STRING);
    EXPECT_EQ(tokens.at(6).lexeme, "Hello world!");
    EXPECT_EQ(tokens.at(tokens.size() - 1).type, TokenType::EOM);
}

TEST(ScannerTests, TestModuleWithNumericLiterals) { // NOLINT(*-throwing-static-initialization, *-owning-memory, *-function-cognitive-complexity)
    namespace fs = std::filesystem;
    const std::string src_file_path{fs::path(__FILE__)
                                          .parent_path()
                                          .append("oberon_src")
                                          .append("NumLiterals.Mod")
                                          .string()};
    auto [tokens, errors] = Scanner::scanSrcFile(src_file_path);
    ASSERT_EQ(tokens.size(), 73);
    // InvalidRealNoIntPart = .2E+4 must be scanned as a dot, followed by an invalid hex int
    // (2E), a plus, and a 4 integer.
    EXPECT_EQ(tokens.at(4).type, TokenType::IDENT);
    EXPECT_EQ(tokens.at(4).lexeme, "InvalidRealNoIntPart");
    EXPECT_EQ(tokens.at(5).type, TokenType::EQUAL);
    EXPECT_EQ(tokens.at(6).type, TokenType::DOT);
    EXPECT_EQ(tokens.at(7).type, TokenType::PLUS);
    EXPECT_EQ(tokens.at(8).type, TokenType::INTEGER);
    EXPECT_EQ(tokens.at(8).lexeme, "4");
    // ValidRealNoDecimalScale must be scanned as a REAL with the appropriate lexeme.
    EXPECT_EQ(tokens.at(15).type, TokenType::IDENT);
    EXPECT_EQ(tokens.at(15).lexeme, "ValidRealNoDecimalScale");
    EXPECT_EQ(tokens.at(16).type, TokenType::EQUAL);
    EXPECT_EQ(tokens.at(17).type, TokenType::REAL);
    EXPECT_EQ(tokens.at(17).lexeme, "23.E+2");
    // ValidRealNoDecimalNScale must be scanned as a REAL with the appropriate lexeme.
    EXPECT_EQ(tokens.at(20).type, TokenType::IDENT);
    EXPECT_EQ(tokens.at(20).lexeme, "ValidRealNoDecimalNoScale");
    EXPECT_EQ(tokens.at(21).type, TokenType::EQUAL);
    EXPECT_EQ(tokens.at(22).type, TokenType::REAL);
    EXPECT_EQ(tokens.at(22).lexeme, "23.");
    // ValidHexInt must be scanned as an INTEGER with the appropriate lexeme.
    EXPECT_EQ(tokens.at(25).type, TokenType::IDENT);
    EXPECT_EQ(tokens.at(25).lexeme, "ValidHexInt");
    EXPECT_EQ(tokens.at(26).type, TokenType::EQUAL);
    EXPECT_EQ(tokens.at(27).type, TokenType::INTEGER);
    EXPECT_EQ(tokens.at(27).lexeme, "87AH");
    // 2AX must be recognized as a valid one-char string. 2A is 42 in base 10 and is the code
    // for the '*'.
    EXPECT_EQ(tokens.at(36).type, TokenType::STRING);
    EXPECT_EQ(tokens.at(36).lexeme, "*");
    EXPECT_EQ(tokens.at(36).line, 9);

    EXPECT_EQ(tokens.at(tokens.size() - 1).type, TokenType::EOM);

    ASSERT_EQ(errors.size(), 4);
    EXPECT_EQ(errors.at(0).line, 3);
    EXPECT_EQ(errors.at(0).column, 41);
    EXPECT_EQ(errors.at(0).msg, "Hexadecimal number must be terminated with an 'H'.");
    EXPECT_EQ(errors.at(1).line, 4);
    EXPECT_EQ(errors.at(1).column, 45);
    EXPECT_EQ(errors.at(1).msg,
              "Real number scale factor must start with an 'E' followed by either a '+' or '-' "
              "signal.");
    EXPECT_EQ(errors.at(2).line, 8);
    EXPECT_EQ(errors.at(2).column, 31);
    EXPECT_EQ(errors.at(2).msg, "Hexadecimal number must be terminated with an 'H'.");
    EXPECT_EQ(errors.at(3).line, 12);
    EXPECT_EQ(errors.at(3).column, 41);
    EXPECT_EQ(errors.at(3).msg, "Real numbers must use only digits between 0 and 9.");
}
