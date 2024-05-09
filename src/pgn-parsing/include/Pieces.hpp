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
    EmptyPiece(const Color &color, const Position &position) {
        type_ = " ";
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    EmptyPiece() = default;
};

using Piece = std::variant<EmptyPiece, Bishop, King, Knight, Pawn, Queen, Rook>;
using Pieces = std::array<std::array<Piece, COLUMNS>, ROWS>;

} // namespace mlp_ha