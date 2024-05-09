#pragma once

#include "BasePiece.hpp"

namespace mlp_ha {

class Pawn : public BasePiece<Pawn> {
  public:
    Pawn(const Color &color, const Position &position) {
        type_ = "P";
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Pawn() = default;
};

} // namespace mlp_ha
