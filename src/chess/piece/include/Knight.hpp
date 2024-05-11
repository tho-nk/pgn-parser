#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class BoardGame;

class Knight : public BasePiece<Knight> {
  public:
    Knight(const Color &color, const Position &position) {
        type_ = PieceType::Knight;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Knight() = default;

    // private:
    bool IsValidBasicMove_(const std::shared_ptr<Square> &square, const Position &toPosition) const;

    bool IsValidAttackMove_(const std::shared_ptr<Square> &square, const Position &toPosition) const;
};

} // namespace mlp_ha
