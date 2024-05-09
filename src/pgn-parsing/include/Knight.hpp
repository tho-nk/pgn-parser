#pragma once

#include "BasePiece.hpp"

namespace mlp_ha {

class Knight : public BasePiece<Knight> {
  public:
    Knight(const Color &color, const Position &position) {
        type_ = "N";
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Knight() = default;
};

} // namespace mlp_ha
