#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class Square;

class Queen : public BasePiece<Queen> {
  public:
    Queen(const Color &color, const Position &position, Square *square) : BasePiece(square) {
        type_ = PieceType::Queen;
        color_ = color;
        position_ = position;
    }

    Queen() = default;

  private:
    friend class BasePiece<Queen>;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const;
};

} // namespace mlp_ha
