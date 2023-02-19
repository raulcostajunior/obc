/**
 * obc - An Oberon-0 to LLVM-IR compiler.
 *
 * The Oberon-0 programming language is a subset of the Oberon language defined
 * in the book, "Compiler Construction" authored by Niklaus Wirth. A revised
 * version of the book can be downloaded from
 * https://people.inf.ethz.ch/wirth/CompilerConstruction/CompilerConstruction1.pdf.
 *
 */
#include <iostream>
#include <string>

#include "CLI/CLI.hpp"
#include "Scanner.hpp"
#include "_version.hpp"

// NOLINTBEGIN(bugprone-exception-escape)
int main(int argc, char **argv) {
    CLI::App app{"An Oberon-0 to LLVM-IR compiler"};

    app.set_version_flag("--version, -v", OBC_VERSION_STR);

    bool lowerCaseKeywords{false};
    app.add_flag("--lower-keywords", lowerCaseKeywords,
                 "Must keywords be lowercase? (in the Oberon-0 spec, keywords are all "
                 "uppercase)");

    std::string srcFile;
    CLI::Option *srcFileOption =
          app.add_option("src_file", srcFile, "Oberon-0 source file to be compiled");
    srcFileOption->required();

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    // For now, we just scan and printout the results.
    Scanner scanner(lowerCaseKeywords);
    const auto res = scanner.scanSrcFile(srcFile);
    // Report on tokens.
    if (res.tokens.empty()) {
        std::cout << "No token found in '" << srcFile << "'." << std::endl;
    } else {
        std::cout << "Scanned " << res.tokens.size()
                  << (res.tokens.size() == 1U ? " token" : " tokens") << " from "
                  << srcFile << ":" << std::endl;
        for (const auto &token : res.tokens) {
            std::cout << token << std::endl;
        }
    }
    // Report on errors.
    if (!res.errors.empty()) {
        if (res.errors.size() == 1) {
            std::cout << "An error happened while scanning '" << srcFile
                      << "':" << std::endl;
        } else {
            std::cout << res.errors.size() << " errors happened while scanning '"
                      << srcFile << "':" << std::endl;
        }
        for (const auto &error : res.errors) {
            std::cout << error << std::endl;
        }
    }
}
// NOLINTEND(bugprone-exception-escape)