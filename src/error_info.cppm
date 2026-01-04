module;

#include <string>

export module error_info;

namespace obc {

    export struct ErrorInfo {
        int line = -1;   // -1 flags for a non-locatable error
        int column = -1; // -1 flags for a non-locatable error
        std::string msg;
    };

    export std::ostream& operator<<(std::ostream& ostream, const ErrorInfo& errInf);

} // namespace obc
