module;

#include <sstream>
#include <string>

export module version;

namespace obc {
    export constexpr int OBC_VERSION_MAJOR{0};
    export constexpr int OBC_VERSION_MINOR{1};
    export constexpr int OBC_VERSION_PATCH{0};

    export std::string inline obcVersion() {
        std::ostringstream ostream;
        ostream << OBC_VERSION_MAJOR << "." << OBC_VERSION_MINOR << "." << OBC_VERSION_PATCH;
        return ostream.str();
    };

} // namespace obc
