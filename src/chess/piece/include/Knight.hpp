#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class Square;

class Knight : public BasePiece<Knight> {
  public:
    Knight(const Color &color, const Position &position, Square *square) : BasePiece(square) {
        type_ = PieceType::Knight;
        color_ = color;
        position_ = position;
    }

     Knight() = default;

  private:
    friend class BasePiece<Knight>;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidMove_(const Position &toPosition) const;
};

} // namespace mlp_ha
