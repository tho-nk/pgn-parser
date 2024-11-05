#include "piece/include/Rook.hpp"
#include "piece/include/Square.hpp"

namespace pgn {
bool Rook::IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    int dRow = toPosition.row - GetPosition().row;
    int dCol = toPosition.col - GetPosition().col;

    // A rook can move only horizontally or vertically
    if (dRow != 0 && dCol != 0) {
        return false;
    }
    return ValidateMove_(dRow, dCol, toPosition, validateKingCheck, GetPosition(),
                         [](const Position &p) { return Square::GetInstance().IsEmptyAt(p); });
}

bool Rook::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition, validateKingCheck);
}

bool Rook::IsValidAttackMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition, validateKingCheck);
}
} // namespace pgn
