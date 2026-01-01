module;

#include <sstream>
#include <string>

export module version;

namespace obc {
    export constexpr int OBC_VERSION_MAJOR{0};
    export constexpr int OBC_VERSION_MINOR{1};
    export constexpr int OBC_VERSION_PATCH{0};

    export std::string inline obcVersion() {
        std::ostringstream ostr;
        ostr << OBC_VERSION_MAJOR << "." << OBC_VERSION_MINOR << "." << OBC_VERSION_PATCH;
        return ostr.str();
    };

} // namespace obc
