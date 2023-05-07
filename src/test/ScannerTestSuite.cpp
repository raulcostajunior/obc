#include <gtest/gtest.h>

#include <filesystem>

#include "Scanner.hpp"

TEST(ScannerTests, TestEmptyFile) {
    // An empty file must have no tokens and no errors.
    const auto res = Scanner::scan("");
    EXPECT_EQ(res.tokens.size(), 0);
    EXPECT_EQ(res.errors.size(), 0);
}

TEST(ScannerTests, TestLowerCaseKeywords) {
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

    const int expectTokens = 41; // Expected number of tokens in src files used in this test.

    // A lexically valid file with lowerCase keywords should be successfully parsed with all
    // the keywords identified with the scanner in lowerCaseKeywords mode.
    auto res = Scanner::scan(lowerCaseSrc, true);
    ASSERT_EQ(res.errors.size(), 0);
    ASSERT_EQ(res.tokens.size(), expectTokens);
    EXPECT_EQ(res.tokens[0].type, TokenType::MODULE);
    EXPECT_EQ(res.tokens[1].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[38].type, TokenType::END);
    EXPECT_EQ(res.tokens[39].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[39].lexeme, "LowerCaseModule");
    EXPECT_EQ(res.tokens[40].type, TokenType::DOT);

    // A lexically valid file with lowerCaseKeywords should be successfully parsed with none
    // of the keywords identified when the scanner is in the default uppercase-keywords mode.
    // All keywords should be identified as token identifiers.
    res = Scanner::scan(lowerCaseSrc);
    ASSERT_EQ(res.errors.size(), 0);
    ASSERT_EQ(res.tokens.size(), expectTokens);
    EXPECT_EQ(res.tokens[0].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[0].lexeme, "module");
    EXPECT_EQ(res.tokens[1].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[38].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[38].lexeme, "end");
    EXPECT_EQ(res.tokens[39].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[39].lexeme, "LowerCaseModule");
    EXPECT_EQ(res.tokens[40].type, TokenType::DOT);

    // A lexically valid file with uppercase keywords should be successfully parsed with all
    // the keywords identified with the scanner in the default uppercase-keywords mode.
    res = Scanner::scan(upperCaseSrc);
    ASSERT_EQ(res.errors.size(), 0);
    ASSERT_EQ(res.tokens.size(), expectTokens);
    EXPECT_EQ(res.tokens[0].type, TokenType::MODULE);
    EXPECT_EQ(res.tokens[1].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[38].type, TokenType::END);
    EXPECT_EQ(res.tokens[39].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[39].lexeme, "LowerCaseModule");
    EXPECT_EQ(res.tokens[40].type, TokenType::DOT);

    // A lexically valid file with uppercase keywords should be successfully parsed with none
    // of the keywords identified when the scanner is in lowercase-keywords mode.
    // All keywords should be identified as token identifiers.
    res = Scanner::scan(upperCaseSrc, true);
    ASSERT_EQ(res.errors.size(), 0);
    ASSERT_EQ(res.tokens.size(), expectTokens);
    EXPECT_EQ(res.tokens[0].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[0].lexeme, "MODULE");
    EXPECT_EQ(res.tokens[1].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[38].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[38].lexeme, "END");
    EXPECT_EQ(res.tokens[39].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[39].lexeme, "LowerCaseModule");
    EXPECT_EQ(res.tokens[40].type, TokenType::DOT);
}

TEST(ScannerTests, TestModuleWithUnfinishedComment) {
    // A source file that finishes with an unfinished comment must trigger an error.
    const std::string unfinishedCommentMsg{"Source module ends in an unfinished comment."};

    const std::string moduleSrc{R"(
MODULE UnfinishedComment;

(* This is a multiline, open ended
comment and should be rejected.
)"};
    auto res = Scanner::scan(moduleSrc);
    ASSERT_EQ(res.tokens.size(), 3);
    EXPECT_EQ(res.tokens[0].type, TokenType::MODULE);
    EXPECT_EQ(res.tokens[1].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[1].lexeme, "UnfinishedComment");
    EXPECT_EQ(res.tokens[2].type, TokenType::SEMICOLON);
    ASSERT_EQ(res.errors.size(), 1);
    EXPECT_EQ(res.errors[0].line, 6);
    EXPECT_EQ(res.errors[0].column, 1);
    EXPECT_EQ(res.errors[0].msg, unfinishedCommentMsg);
}

TEST(ScannerTests, TestModuleWithInvalidSymbol) {
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

    auto res = Scanner::scan(invalidSymbolSrc);
    ASSERT_EQ(res.tokens.size(), 16);
    EXPECT_EQ(res.tokens[0].type, TokenType::MODULE);
    EXPECT_EQ(res.tokens[1].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[1].lexeme, "WithInvalidSymbol");
    // Tokens 7 and 8 are the ones around the invalid symbol in the source.
    // We verify if they have been properly scanned.
    EXPECT_EQ(res.tokens[7].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[7].lexeme, "INTEGER");
    EXPECT_EQ(res.tokens[8].type, TokenType::SEMICOLON);
    EXPECT_EQ(res.tokens[15].type, TokenType::DOT);

    ASSERT_EQ(res.errors.size(), 1);
    // The R-String for the source starts with a new line, so the MODULE line
    // is already line 2 in the source - that's the reason for line with the
    // invalid symbol being line 8.
    EXPECT_EQ(res.errors[0].line, 8);
    EXPECT_EQ(res.errors[0].msg, std::string{"Unexpected character, '?' found."});
}

TEST(ScannerTests, TestModuleWithStringLiteral) {
    namespace fs = std::filesystem;
    const std::string src_file_path{
          fs::path(__FILE__).parent_path().append("oberon_src").append("Hello.Mod")};
    auto res = Scanner::scanSrcFile(src_file_path);
    ASSERT_EQ(res.tokens.size(), 12);
    EXPECT_EQ(res.tokens[0].type, TokenType::MODULE);
    EXPECT_EQ(res.tokens[1].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[1].lexeme, "Hello");
    EXPECT_EQ(res.tokens[3].type, TokenType::BEGIN);
    EXPECT_EQ(res.tokens[4].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[4].lexeme, "WriteLn");
    EXPECT_EQ(res.tokens[5].type, TokenType::LEFT_PAREN);
    EXPECT_EQ(res.tokens[6].type, TokenType::STRING);
    EXPECT_EQ(res.tokens[6].lexeme, "Hello world!");
}