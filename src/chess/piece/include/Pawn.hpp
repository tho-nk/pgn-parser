#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class Square;

class Pawn : public BasePiece<Pawn> {
  public:
    Pawn(const Color &color, const Position &position) : BasePiece() {
        type_ = PieceType::Pawn;
        color_ = color;
        position_ = position;
    }

    Pawn() = default;

  private:
    friend class BasePiece<Pawn>;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;
};

} // namespace mlp_ha
