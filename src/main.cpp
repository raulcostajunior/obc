/**
 * oboc - An Oberon-O to WebAssembly compiler.
 *
 * The Oberon-O programming language is a subset of the Oberon language defined
 * in the book, "Compiler Construction" authored by Niklaus Wirth. A revised
 * version of the book can be downloaded from
 * https://people.inf.ethz.ch/wirth/CompilerConstruction/CompilerConstruction1.pdf.
 *
 */
#include <string>

#include "CLI11.hpp"
#include "_version.hpp"

// NOLINTBEGIN(bugprone-exception-escape)
int main(int argc, char **argv) {
    CLI::App app{"An Oberon-O to WebAssembly compiler"};

    app.set_version_flag("--version", OBOC_VERSION_STR);

    std::string srcFile;
    CLI::Option *srcFileOption = app.add_option(
          "src_file", srcFile, "Oberon-O source file to be compiled");
    srcFileOption->required();

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }
}
// NOLINTEND(bugprone-exception-escape)