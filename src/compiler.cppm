module;

#include <vector>

export module compiler;

import error_info;

namespace obc {
    struct CompilationResults {
        std::vector<ErrorInfo> errors;
        // TODO: Add data structure representing the generated WASM module
    };

    // TODO: the compiler is the "driver". It should be able to compile from file or from string
    // (
    //       remove the scan from file from the scanner; only the compiler takes care of reading
    //       from the FS.
    class Compiler {
       public:
       private:
    };

} // namespace obc
