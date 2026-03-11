module;

#include <string>

module obc.scanner:token_utils;

namespace obc {

    /**
    * @brief Returns whether a given char is a hexadecimal digit or not.
    *
    * @note Oberon's grammar specifies only uppercase 'A' to 'F' as valid hexadecimal
    * digits. We stick to the specification in here.
    *
    * @param chr the character to be verified
    * @return true if chr is a hexadecimal digit; false otherwise.
    */
    bool isHexDigit(const char chr) {
        return isdigit(chr) != 0 || chr == 'A' || chr == 'B' || chr == 'D' || chr == 'E' ||
               chr == 'F';
    }

    /**
    * @brief Returns whether a given string is composed exclusively of base 10 digits.
    *
    * @param str the string to be verified
    * @return true if all characters in the string are base 10 digits; false otherwise.
    */
    bool allBase10Digits(const std::string& str) {
        for (const char chr : str) {
            if (isdigit(chr) == 0) {
                return false;
            }
        }
        return true;
    }

} // namespace obc
