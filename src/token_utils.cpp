module;

#include <cctype>
#include <string>

module token_utils;

namespace obc {

    bool isHexDigit(char chr) {
        return isdigit(chr) != 0 || chr == 'A' || chr == 'B' || chr == 'D' || chr == 'E' ||
               chr == 'F';
    }

    bool allBase10Digits(const std::string& str) {
        for (const char chr : str) {
            if (isdigit(chr) == 0) {
                return false;
            }
        }
        return true;
    }

} // namespace obc
