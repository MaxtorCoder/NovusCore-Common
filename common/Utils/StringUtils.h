#pragma once

#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include <iomanip>
#include "../NovusTypes.h"
#ifdef _WINDOWS
#include <Windows.h>
#endif

namespace StringUtils
{
inline std::string BytesToHexStr(u8* data, size_t length)
{
    std::stringstream ss;
    ss << std::hex;

    for (int i(0); i < length; ++i)
        ss << std::setw(2) << std::setfill('0') << (int)data[i];

    return ss.str();
}

inline int HexCharToInt(char input)
{
    if (input >= '0' && input <= '9')
        return input - '0';
    if (input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if (input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");
}
// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
inline void HexStrToBytes(const char* src, u8* target)
{
    while (*src && src[1])
    {
        *(target++) = HexCharToInt(*src) * 16 + HexCharToInt(src[1]);
        src += 2;
    }
}

inline std::string GetLineFromCin()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}

inline std::vector<std::string> SplitString(std::string string, char delim = ' ')
{
    std::vector<std::string> results;
    std::stringstream ss(string);
    std::string token;

    while (std::getline(ss, token, delim))
    {
        results.push_back(token);
    }

    return results;
}

inline std::string EscapeString(std::string const& string)
{
    std::size_t n = string.length();
    std::string escaped;
    escaped.reserve(n * 2); // pessimistic preallocation

    for (std::size_t i = 0; i < n; ++i)
    {
        if (string[i] == '\\' || string[i] == '\'')
            escaped += '\\';
        escaped += string[i];
    }
    return escaped;
}

template <typename... Args>
inline i32 FormatString(char* buffer, size_t bufferSize, char const* format, Args... args)
{
    i32 length = -1;
#ifdef _WIN32
    length = sprintf_s(buffer, bufferSize, format, args...);
#else
    (void)bufferSize; // Get rid of warning
    length = sprintf(buffer, format, args...);
#endif
    assert(length > -1);
    return length;
}

// FNV-1a 32bit hashing algorithm.
constexpr u32 fnv1a_32(char const* s, std::size_t count)
{
    return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
}

#ifdef _WINDOWS
inline std::wstring StringToWString(const std::string& as)
{
    // deal with trivial case of empty string
    if (as.empty())    return std::wstring();

    // determine required length of new string
    size_t reqLength = ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0);

    // construct new string of required length
    std::wstring ret(reqLength, L'\0');

    // convert old string to new string
    ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length());

    // return new string ( compiler should optimize this away )
    return ret;
}
#endif

inline bool EndsWith(std::string const& fullString, std::string const& ending) 
{
    if (fullString.length() >= ending.length()) 
    {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else 
    {
        return false;
    }
}
} // namespace StringUtils

constexpr u32 operator"" _h(char const* s, std::size_t count)
{
    return StringUtils::fnv1a_32(s, count);
}