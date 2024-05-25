#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class Square;

class Pawn : public BasePiece<Pawn> {
  public:
    Pawn(const Color &color, const Position &position) {
        type_ = PieceType::Pawn;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &p) { return {}; }

    Pawn() = default;

  private:
    friend class BasePiece<Pawn>;
    bool IsValidBasicMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;
};

} // namespace mlp_ha
