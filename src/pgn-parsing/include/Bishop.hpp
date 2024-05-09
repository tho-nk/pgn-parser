#pragma once

#include "BasePiece.hpp"

namespace mlp_ha {

class Bishop : public BasePiece<Bishop> {
  public:
    Bishop(const Color &color, const Position &position) {
        type_ = "B";
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Bishop() = default;
};

} // namespace mlp_ha
