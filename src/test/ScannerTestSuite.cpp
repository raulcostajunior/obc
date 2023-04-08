#include <gtest/gtest.h>

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
    // of the keywords identified when the scanner is in the default uppercase keywords mode.
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
    // the keywords identified with the scanner in the default uppercase keywords mode.
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
    // of the keywords identified when the scanner is in lowercase keywords mode.
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
    // A source file that finishes with an unfinished comment.
    const std::string unfinishedCommentMsg{"Source module ends in an unfinished comment."};

    // First using the standard upper case keywords from Oberon.
    const std::string upKeywordsSrc{R"(
MODULE UnfinishedComment;

(* This is a multiline, open ended
comment and should be rejected.
)"};
    auto res = Scanner::scan(upKeywordsSrc);
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