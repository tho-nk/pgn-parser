#pragma once
#include "BasePiece.hpp"

namespace mlp_ha {

class BoardGame;

class Queen : public BasePiece<Queen> {
  public:
    Queen(const Color &color, const Position &position) {
        type_ = PieceType::Queen;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Queen() = default;

    // private:
    bool IsValidBasicMove_(const Position &position) const;
};

} // namespace mlp_ha
