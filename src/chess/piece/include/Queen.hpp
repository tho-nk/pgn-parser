#pragma once

#include "piece/include/BasePiece.hpp"

namespace pgn {

class Queen : public BasePiece<Queen> {
  public:
    Queen(const Color &color, const Position &position) : BasePiece<Queen>() {
        SetType(PieceType::Queen);
        SetColor(color);
        SetPosition(position);
    }

    Queen() = default;

    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const;
};

} // namespace pgn
