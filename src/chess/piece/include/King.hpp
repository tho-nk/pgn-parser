#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class Square;

class King : public BasePiece<King> {
  public:
    King(const Color &color, const Position &position, const std::shared_ptr<Square> &square = nullptr)
        : BasePiece(square) {
        type_ = PieceType::King;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    King() = default;

  private:
    friend class BasePiece<King>;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;
    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;
};

} // namespace mlp_ha
