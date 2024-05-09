#pragma once
#include "BasePiece.hpp"

namespace mlp_ha {

class Queen : public BasePiece<Queen> {
  public:
    Queen(const Color &color, const Position &position) {
        type_ = PieceType::Queen;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    Queen() = default;
};

} // namespace mlp_ha
