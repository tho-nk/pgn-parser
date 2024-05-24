#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class Square;

class Rook : public BasePiece<Rook> {
  public:
    Rook(const Color &color, const Position &position, const std::shared_ptr<Square> &square) {
        type_ = PieceType::Rook;
        color_ = color;
        position_ = position;
        square_ = square;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Rook() = default;

  private:
    friend class BasePiece<Rook>;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;
};

} // namespace mlp_ha
