#pragma once

#include "piece/include/BasePiece.hpp"

namespace pgn {

class EmptyPiece : public BasePiece {
  public:
    EmptyPiece(const Position &position) : BasePiece() {
        type_ = PieceType::Undefined;
        color_ = Color::Undefined;
        position_ = position;
    }

    EmptyPiece() = default;
};
} // namespace pgn