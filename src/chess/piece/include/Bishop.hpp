#pragma once

#include "piece/include/BasePiece.hpp"

namespace pgn {

class Bishop : public BasePiece {
  public:
    Bishop(const Color &color, const Position &position) : BasePiece() {
        type_ = PieceType::Bishop;
        color_ = color;
        position_ = position;
    }

    Bishop() = default;

  private:
    friend class BasePiece;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const;
};

} // namespace pgn
