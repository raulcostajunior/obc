#include <gtest/gtest.h>

#include "Scanner.hpp"

TEST(ScannerTests, TestEmptyFile) {
    // An empty file must have no tokens and no errors.
    const auto scan_results = Scanner::scan("");
    ASSERT_EQ(scan_results.tokens.size(), 0);
    ASSERT_EQ(scan_results.errors.size(), 0);
}