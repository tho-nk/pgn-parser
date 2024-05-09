#pragma once
#include "BasePiece.hpp"

namespace mlp_ha {

class King : public BasePiece<King> {
  public:
    King(const Color &color, const Position &position) {
        type_ = PieceType::King;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    King() = default;
};

} // namespace mlp_ha
