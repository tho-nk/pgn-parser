#pragma once
#include "BasePiece.hpp"

namespace mlp_ha {

class Rook : public BasePiece<Rook> {
  public:
    Rook(const Color &color, const Position &position) {
        type_ = "R";
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Rook() = default;
};

} // namespace mlp_ha
