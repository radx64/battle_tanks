#pragma once

#include <array>
#include <string_view>

namespace engine 
{
template <std::size_t N, std::size_t M>
constexpr auto concat(const char (&a)[N], const char (&b)[M])
{
    std::string result; 
    result.reserve(N + M - 1);  // -1 to avoid duplicate null terminator
    result.append(a, N - 1);  // -1 to avoid null terminator
    result.append(b, M);
    return result;
}
}  // namespace engine
