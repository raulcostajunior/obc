module;

#include <iostream>

module error_info;

namespace obc {

    std::ostream& operator<<(std::ostream& ostream, const ErrorInfo& errInf) {
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
