#pragma once
#include <string>
#include <vector>

namespace mlp_ha {

enum class Color { White, Black, Undefined };

struct Position {
    int x;
    int y;
};

using FromPosition = Position;
using ToPosition = Position;
using Positions = std::vector<Position>;
using PieceType = std::string;

} // namespace mlp_ha
