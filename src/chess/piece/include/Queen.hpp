#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class Square;

class Queen : public BasePiece<Queen> {
  public:
    Queen(const Color &color, const Position &position, const std::shared_ptr<Square> &square) : BasePiece(square) {
        type_ = PieceType::Queen;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Queen() = default;

  private:
    friend class BasePiece<Queen>;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;
};

} // namespace mlp_ha
