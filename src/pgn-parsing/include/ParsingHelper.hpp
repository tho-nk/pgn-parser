#pragma once

#include <filesystem>
#include <iostream>
#include <queue>
#include <string>
#include <string_view>

namespace mlp_ha {
bool IsBalanced(std::string_view str);

std::queue<std::string> ParseFile(const std::filesystem::path &path);

} // namespace mlp_ha