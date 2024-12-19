#pragma once

#include "piece/include/BasePiece.hpp"

namespace pgn {

class Rook : public BasePiece {
  public:
    Rook(const Color &color, const Position &position) : BasePiece() {
        type_ = PieceType::Rook;
        color_ = color;
        position_ = position;
    }

    Rook() = default;

  private:
    friend class BasePiece;
    bool IsValidBasicMove_(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidAttackMove_(const Position &toPosition,
                            const std::optional<Position> &validateKingCheck = std::nullopt) const;

    bool IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const;
};

} // namespace pgn
