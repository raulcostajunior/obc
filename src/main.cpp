/**
 * obc - An Oberon-07 to LLVM-IR compiler.
 *
 * The Oberon-07 programming language is described in
 * https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf
 */
#include <iostream>
#include <string>

#include "CLI/CLI.hpp"
#include "Parser.hpp"
#include "Scanner.hpp"
#include "Version.hpp"

// NOLINTBEGIN(bugprone-exception-escape)
int main(int argc, char **argv) {
    CLI::App app{"An Oberon-07 to LLVM-IR compiler"};

    app.set_version_flag("--version, -v", obcVersion());

    bool lowerCaseKeywords{false};
    app.add_flag("--lower-keywords", lowerCaseKeywords,
                 "Must keywords be lowercase? (in the Oberon-07 spec, keywords are all "
                 "uppercase)");

    std::string srcFile;
    CLI::Option *srcFileOption =
          app.add_option("src_file", srcFile, "Oberon-07 source file to be compiled");
    srcFileOption->required();

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    // For now, we just scan and printout the results.
    auto res = Scanner::scanSrcFile(srcFile, lowerCaseKeywords);
    // Report on tokens.
    if (res.tokens.empty()) {
        std::cout << "No token found in '" << srcFile << "'.\n";
    } else {
        std::cout << "Scanned " << res.tokens.size()
                  << (res.tokens.size() == 1U ? " token" : " tokens") << " from " << srcFile
                  << ":\n";
        for (const auto &token : res.tokens) {
            std::cout << token << "\n";
        }
    }
    // Report on errors.
    if (!res.errors.empty()) {
        if (res.errors.size() == 1) {
            std::cout << "An error happened while scanning '" << srcFile << "':\n";
        } else {
            std::cout << res.errors.size() << " errors happened while scanning '" << srcFile
                      << "':\n";
        }
        for (const auto &error : res.errors) {
            std::cout << error << "\n";
        }
    }

    Parser parser{std::move(res.tokens)};
}
// NOLINTEND(bugprone-exception-escape)
