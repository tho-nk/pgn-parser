#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class BoardGame;

class King : public BasePiece<King> {
  public:
    King(const Color &color, const Position &position) {
        type_ = PieceType::King;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    King() = default;

    // private:
    bool IsValidBasicMove_(const std::shared_ptr<Square> &square, const Position &toPosition) const;
    bool IsValidAttackMove_(const std::shared_ptr<Square> &square, const Position &toPosition) const;
};

} // namespace mlp_ha
