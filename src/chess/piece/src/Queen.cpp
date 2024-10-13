#include "piece/include/Queen.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {
bool Queen::IsValidMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    int dRow = toPosition.row - GetPosition().row;
    int dCol = toPosition.col - GetPosition().col;

    // A queen can move only horizontally, vertically, or diagonally
    if ((dRow != 0 && dCol != 0) && (std::abs(dRow) != std::abs(dCol))) {
        return false;
    }

    // Normalize the change in row and column to -1, 0, or 1
    if (dRow != 0) {
        dRow /= std::abs(dRow);
    }
    if (dCol != 0) {
        dCol /= std::abs(dCol);
    }

    // Iterate to the destination, and check if there is any obstacle
    Position p{GetPosition().row + dRow, GetPosition().col + dCol};
    while (p.IsValid()) {
        if (p == toPosition) {
            return true;
        }
        if (!std::holds_alternative<EmptyPiece>(square_->GetPieces()[p.row][p.col])) {
            if (validateKingCheck != std::nullopt) {
                if (!(validateKingCheck.value() == p)) {
                    return false;
                }
            } else {
                return false;
            }
        }
        p.Shift(dRow, dCol);
    }
    return false;
}

bool Queen::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition, validateKingCheck);
}

bool Queen::IsValidAttackMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition, validateKingCheck);
}
} // namespace mlp_ha
