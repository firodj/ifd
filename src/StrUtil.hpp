#include <string>
#include <algorithm>

namespace ifd
{
////////////////////////////////////////////////////////////////////////////////

//
//  Lowercases string
//
template <typename T>
std::basic_string<T> lowercase(const std::basic_string<T>& s)
{
    std::basic_string<T> s2 = s;
    std::transform(s2.begin(), s2.end(), s2.begin(),
        [](const T v){ return static_cast<T>(std::tolower(v)); });
    return s2;
}

//
// Uppercases string
//
template <typename T>
std::basic_string<T> uppercase(const std::basic_string<T>& s)
{
    std::basic_string<T> s2 = s;
    std::transform(s2.begin(), s2.end(), s2.begin(),
        [](const T v){ return static_cast<T>(std::toupper(v)); });
    return s2;
}

////////////////////////////////////////////////////////////////////////////////
};