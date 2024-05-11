#pragma once

#include "piece/include/BasePiece.hpp"

namespace mlp_ha {

class BoardGame;

class Bishop : public BasePiece<Bishop> {
  public:
    Bishop(const Color &color, const Position &position) {
        type_ = PieceType::Bishop;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Bishop() = default;

    // private:
    bool IsValidBasicMove_(std::shared_ptr<Square> square, const Position &toPosition) const;

    bool IsValidAttackMove_(std::shared_ptr<Square> square, const Position &toPosition) const;
};

} // namespace mlp_ha
