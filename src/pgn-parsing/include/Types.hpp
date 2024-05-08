#pragma once

namespace mlp_ha {

enum class Color { White, Black };

struct Position {
    int x;
    int y;
};

using FromPosition = Position;
using ToPosition = Position;

} // namespace mlp_ha
