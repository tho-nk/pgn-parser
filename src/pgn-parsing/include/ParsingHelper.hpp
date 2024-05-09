#pragma once

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <queue>
#include <string>
#include <string_view>

namespace mlp_ha {
namespace helper {
bool IsBalanced(std::string_view str);

size_t GetNextNonSpace(std::string_view text, size_t index);

size_t GetNextSpace(std::string_view text, size_t index);

void GetComment(std::string_view text, std::string &comment, size_t &index);

inline void LeftTrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

inline void RightTrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

inline void TrimSpace(std::string &s) {
    RightTrim(s);
    LeftTrim(s);
}

void Remove3Dot(std::string &str);

std::queue<std::string> ParseFile(const std::filesystem::path &path);
} // namespace helper
} // namespace mlp_ha