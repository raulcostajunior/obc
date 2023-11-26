
#ifndef VERSION_HPP
#define VERSION_HPP

#include <sstream>
#include <string>

const int OBC_VERSION_MAJOR{0};
const int OBC_VERSION_MINOR{1};
const int OBC_VERSION_PATCH{0};

std::string obcVersion() {
    std::ostringstream ostr;
    ostr << OBC_VERSION_MAJOR << "." << OBC_VERSION_MINOR << "." << OBC_VERSION_PATCH;
    return ostr.str();
}


#endif
