#pragma once

#include "piece/include/BasePiece.hpp"

namespace pgn {

class EmptyPiece : public BasePiece<EmptyPiece> {
  public:
    EmptyPiece(const Position &position) : BasePiece<EmptyPiece>() {
        SetType(PieceType::Undefined);
        SetColor(Color::Undefined);
        SetPosition(position);
    }

    EmptyPiece() = default;

    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const {
        return false;
    }

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const {
        return false;
    }
};
} // namespace pgn
