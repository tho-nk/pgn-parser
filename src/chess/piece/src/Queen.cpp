#include "piece/include/Queen.hpp"
#include "common/include/GameHelper.hpp"
#include "piece/include/Square.hpp"

namespace mlp_ha {
bool Queen::IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    int dRow = toPosition.row - GetPosition().row;
    int dCol = toPosition.col - GetPosition().col;

    // A queen can move only horizontally, vertically, or diagonally
    if ((dRow != 0 && dCol != 0) && (std::abs(dRow) != std::abs(dCol))) {
        return false;
    }

    return ValidateMove(dRow, dCol, toPosition, validateKingCheck, square_, GetPosition());
}

bool Queen::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition, validateKingCheck);
}

bool Queen::IsValidAttackMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition, validateKingCheck);
}
} // namespace mlp_ha
