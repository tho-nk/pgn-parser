#pragma once

#include "Bishop.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"
#include "Queen.hpp"
#include "Rook.hpp"
#include <array>
#include <variant>

namespace mlp_ha {
constexpr size_t COLUMNS = 8;
constexpr size_t ROWS = 8;

class EmptyPiece : public BasePiece<EmptyPiece> {
  public:
    EmptyPiece() = default;
    ~EmptyPiece() = default;

    Positions ComputeFromPositions(const std::string &) { return {}; }
};

using Piece = std::variant<EmptyPiece, Bishop, King, Knight, Pawn, Queen, Rook>;
using Pieces = std::array<std::array<Piece, COLUMNS>, ROWS>;

} // namespace mlp_ha