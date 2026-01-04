module;

#include <string>

export module token_utils;

namespace obc {

    /**
     * @brief Returns whether a given char is an hexadecimal digit or not.
     *
     * @note Oberon's grammar specifies only uppercase 'A' to 'F' as valid hexadecimal
     * digits. We stick to the specification in here.
     *
     * @param chr the character to be verified
     * @return true if chr is an hexadecimal digit; false otherwise.
     */
    export bool isHexDigit(char chr);

    /**
     * @brief Returns whether a given string is composed exclusively of base 10 digits.
     *
     * @param str the string to be verified
     * @return true if all characters in the string are base 10 digits; false otherwise.
     */
    export bool allBase10Digits(const std::string& str);

} // namespace obc
