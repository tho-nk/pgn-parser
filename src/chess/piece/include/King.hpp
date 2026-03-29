#pragma once

#include "piece/include/BasePiece.hpp"

namespace pgn {

class King : public BasePiece<King> {
  public:
    King(const Color &color, const Position &position) : BasePiece<King>() {
        SetType(PieceType::King);
        SetColor(color);
        SetPosition(position);
    }

    King() = default;

    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;
    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidMove_(const Position &toPosition) const;
};

} // namespace pgn
