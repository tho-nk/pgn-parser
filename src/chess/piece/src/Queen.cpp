#include "piece/include/Queen.hpp"
#include "piece/include/Square.hpp"

namespace pgn {
bool Queen::IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    int dRow = toPosition.row - GetPosition().row;
    int dCol = toPosition.col - GetPosition().col;

    // A queen can move only horizontally, vertically, or diagonally
    if ((dRow != 0 && dCol != 0) && (std::abs(dRow) != std::abs(dCol))) {
        return false;
    }

    return ValidateMove_(dRow, dCol, toPosition, validateKingCheck, GetPosition(),
                         [](const Position &p) { return Square::GetInstance().IsEmptyAt(p); });
}

bool Queen::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition, validateKingCheck);
}

bool Queen::IsValidAttackMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition, validateKingCheck);
}
} // namespace pgn
