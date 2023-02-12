#include "ErrorInfo.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& ostr, const ErrorInfo& errInf) {
    if (errInf.line < 0 || errInf.column < 0) {
        ostr << errInf.msg;
    } else {
        ostr << "(lin " << errInf.line << ", col " << errInf.column
             << "): " << errInf.msg;
    }
    return ostr;
}
