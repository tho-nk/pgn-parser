#pragma once

#include "piece/include/BasePiece.hpp"

namespace pgn {

class Pawn : public BasePiece<Pawn> {
  public:
    Pawn(const Color &color, const Position &position) : BasePiece<Pawn>() {
        SetType(PieceType::Pawn);
        SetColor(color);
        SetPosition(position);
    }

    Pawn() = default;

    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;
};

} // namespace pgn
