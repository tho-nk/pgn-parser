#pragma once

#include "piece/include/Pieces.hpp"
#include <optional>
#include <variant>

namespace mlp_ha {

// This validation helper function is applied for Queen, Rook, and Bishop
bool ValidateMove(int dRow, int dCol, const Position &toPosition, const std::optional<Position> &validateKingCheck,
                  Position p);
} // namespace mlp_ha
