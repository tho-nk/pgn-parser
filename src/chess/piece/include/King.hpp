#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class Square;

class King : public BasePiece<King> {
  public:
    King(const Color &color, const Position &position, Square *square) : BasePiece(square) {
        type_ = PieceType::King;
        color_ = color;
        position_ = position;
    }

    King() = default;

  private:
    friend class BasePiece<King>;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;
    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidMove_(const Position &toPosition) const;
};

} // namespace mlp_ha
