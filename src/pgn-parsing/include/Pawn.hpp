#pragma once

#include "BasePiece.hpp"

namespace mlp_ha {

class BoardGame;

class Pawn : public BasePiece<Pawn> {
  public:
    Pawn(const Color &color, const Position &position) {
        type_ = PieceType::Pawn;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &p) {
        std::cout << "Pawn::ComputeFromPositions:= " << p << std::endl;
        return {};
    }

    Pawn() = default;

    // private:
    bool IsValidBasicMove_(const Position &position) const;
};

} // namespace mlp_ha
