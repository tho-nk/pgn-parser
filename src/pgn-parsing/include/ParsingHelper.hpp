#pragma once

#include <filesystem>
#include <iostream>
#include <queue>
#include <string>
#include <string_view>

namespace mlp_ha {
bool IsBalanced(std::string_view str);

size_t getNextNonSpace(std::string_view text, size_t index);

size_t getNextSpace(std::string_view text, size_t index);

void getComment(std::string_view text, std::string &comment, size_t &index);

std::queue<std::string> ParseFile(const std::filesystem::path &path);

} // namespace mlp_ha