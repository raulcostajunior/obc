#ifndef OBC_ERRORINFO_HPP
#define OBC_ERRORINFO_HPP

#include <string>

struct ErrorInfo {
    int line = -1;   // -1 flags for a non-locatable error
    int column = -1; // -1 flags for a non-locatable error
    std::string msg;
};

#endif // OBC_ERRORINFO_HPP
