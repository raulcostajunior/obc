module;

#include <iostream>
#include <string>

export module obc.error_info;

namespace obc {

    export struct ErrorInfo {
        int line = -1;   // -1 flags for a non-locatable error
        int column = -1; // -1 flags for a non-locatable error
        std::string msg;
    };

    export std::ostream& operator<<(std::ostream& ostream, const ErrorInfo& errInf) {
        if (errInf.line < 0 && errInf.column < 0) {
            ostream << errInf.msg;
        } else if (errInf.column < 0) {
            ostream << "(lin " << errInf.line << "): " << errInf.msg;
        } else {
            ostream << "(lin " << errInf.line << ", col " << errInf.column << "): " << errInf.msg;
        }
        return ostream;
    }

} // namespace obc


