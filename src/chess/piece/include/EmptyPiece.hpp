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

  private:
    friend class BasePiece;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;
};
} // namespace pgn