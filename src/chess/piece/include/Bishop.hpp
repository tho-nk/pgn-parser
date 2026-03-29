#pragma once

#include "piece/include/BasePiece.hpp"

namespace pgn {

class Bishop : public BasePiece<Bishop> {
  public:
    Bishop(const Color &color, const Position &position) : BasePiece<Bishop>() {
        SetType(PieceType::Bishop);
        SetColor(color);
        SetPosition(position);
    }

    Bishop() = default;

    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const;
};

} // namespace pgn
