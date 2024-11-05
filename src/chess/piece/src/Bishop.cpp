#include "piece/include/Bishop.hpp"
#include "piece/include/Square.hpp"

namespace pgn {

bool Bishop::IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    int dRow = toPosition.row - GetPosition().row;
    int dCol = toPosition.col - GetPosition().col;

    // A bishop can move only diagonally, so the dRow and dCol must be equal
    if (std::abs(dRow) != std::abs(dCol)) {
        return false;
    }
    return ValidateMove_(dRow, dCol, toPosition, validateKingCheck, GetPosition(),
                         [](const Position &p) { return Square::GetInstance().IsEmptyAt(p); });
}

bool Bishop::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition, validateKingCheck);
}

bool Bishop::IsValidAttackMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition, validateKingCheck);
}
} // namespace pgn
