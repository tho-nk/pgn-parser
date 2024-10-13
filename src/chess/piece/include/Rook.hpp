#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class Square;

class Rook : public BasePiece<Rook> {
  public:
    Rook(const Color &color, const Position &position, Square *square) : BasePiece(square) {
        type_ = PieceType::Rook;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Rook() = default;

  private:
    friend class BasePiece<Rook>;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const;
};

} // namespace mlp_ha
