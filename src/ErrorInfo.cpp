#include "ErrorInfo.hpp"

#include <iostream>

namespace obc {

    std::ostream& operator<<(std::ostream& ostr, const ErrorInfo& errInf) {
        if (errInf.line < 0 && errInf.column < 0) {
            ostr << errInf.msg;
        } else if (errInf.column < 0) {
            ostr << "(lin " << errInf.line << "): " << errInf.msg;
        } else {
            ostr << "(lin " << errInf.line << ", col " << errInf.column << "): " << errInf.msg;
        }
        return ostr;
    }

} // namespace obc
