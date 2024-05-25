#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class Square;

class Knight : public BasePiece<Knight> {
  public:
    Knight(const Color &color, const Position &position) {
        type_ = PieceType::Knight;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Knight() = default;

  private:
    friend class BasePiece<Knight>;
    bool IsValidBasicMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;
};

} // namespace mlp_ha
