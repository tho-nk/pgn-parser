#pragma once

#include "piece/include/BasePiece.hpp"

namespace pgn {

class Knight : public BasePiece<Knight> {
  public:
    Knight(const Color &color, const Position &position) : BasePiece<Knight>() {
        SetType(PieceType::Knight);
        SetColor(color);
        SetPosition(position);
    }

    Knight() = default;

    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidMove_(const Position &toPosition) const;
};

} // namespace pgn
