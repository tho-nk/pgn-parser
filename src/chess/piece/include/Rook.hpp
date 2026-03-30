#pragma once

#include "piece/include/BasePiece.hpp"

namespace pgn {

class Rook : public BasePiece<Rook> {
  public:
    Rook(const Color &color, const Position &position) : BasePiece<Rook>() {
        SetType(PieceType::Rook);
        SetColor(color);
        SetPosition(position);
    }

    Rook() = default;

    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const;
};

} // namespace pgn
