#pragma once

#include "piece/include/BasePiece.hpp"

namespace pgn {

class EmptyPiece : public BasePiece<EmptyPiece> {
  public:
    EmptyPiece(const Color &color, const Position &position) : BasePiece() {
        type_ = PieceType::Undefined;
        color_ = color;
        position_ = position;
    }

    EmptyPiece() = default;

  private:
    friend class BasePiece<EmptyPiece>;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;
};
} // namespace pgn