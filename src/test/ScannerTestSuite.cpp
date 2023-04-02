#include <gtest/gtest.h>

#include "Scanner.hpp"

TEST(ScannerTests, TestEmptyFile) {
    // An empty file must have no tokens and no errors.
    const auto res = Scanner::scan("");
    EXPECT_EQ(res.tokens.size(), 0);
    EXPECT_EQ(res.errors.size(), 0);
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
    // Using the Oberon standard of all upper keywords.
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

    // Using the lower case keywords "extension" from obc, "MODULE" must be scanned as an IDENT
    // token.
    res = Scanner::scan(upKeywordsSrc, true);
    ASSERT_EQ(res.tokens.size(), 3);
    EXPECT_EQ(res.tokens[0].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[0].lexeme, "MODULE");
    EXPECT_EQ(res.tokens[1].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[1].lexeme, "UnfinishedComment");
    EXPECT_EQ(res.tokens[2].type, TokenType::SEMICOLON);
    ASSERT_EQ(res.errors.size(), 1);
    EXPECT_EQ(res.errors[0].line, 6);
    EXPECT_EQ(res.errors[0].column, 1);
    EXPECT_EQ(res.errors[0].msg, unfinishedCommentMsg);

    // Then using the source with lower case keywords.
    const std::string lowKeywordsSrc = R"(
module UnfinishedComment;

(* This is a multiline, open ended
comment and should be rejected.
)";
    // In the default upper case keywords mode, instead of a MODULE token, the scanner must
    // identify an IDENT token.
    res = Scanner::scan(lowKeywordsSrc);
    ASSERT_EQ(res.tokens.size(), 3);
    EXPECT_EQ(res.tokens[0].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[0].lexeme, "module");
    EXPECT_EQ(res.tokens[1].type, TokenType::IDENT);
    EXPECT_EQ(res.tokens[1].lexeme, "UnfinishedComment");
    EXPECT_EQ(res.tokens[2].type, TokenType::SEMICOLON);
    ASSERT_EQ(res.errors.size(), 1);
    EXPECT_EQ(res.errors[0].line, 6);
    EXPECT_EQ(res.errors[0].column, 1);
    EXPECT_EQ(res.errors[0].msg, unfinishedCommentMsg);

    // In the lower case keywords mode, the MODULE token must be identified.
    res = Scanner::scan(lowKeywordsSrc, true);
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