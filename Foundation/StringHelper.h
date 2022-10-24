#pragma once

#ifndef __STRINGHELPER_H___
#define __STRINGHELPER_H___

#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>

namespace dios::foundation
{

class StringHelper
{
public:
    StringHelper() = delete;
    ~StringHelper() = delete;

    // trim from start
    static inline std::string& ltrim(std::string& s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return s;
    }

    // trim from end
    static inline std::string& rtrim(std::string& s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
    }

    // trim from both ends
    static inline std::string& trim(std::string& s) {
            return ltrim(rtrim(s));
    }

    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    static bool stringToNumber(const std::string& input, T& value)
    {
        return (std::istringstream(input) >> value).fail();
    }

    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    static const std::string& numberToString(T number, std::string& output)
    {
        std::ostringstream oss;
        oss << number;
        output = std::move(oss.str());
        return output;
    }
};

}

#endif /* __STRINGHELPER_H___ */
