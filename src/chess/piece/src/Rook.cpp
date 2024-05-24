#include "piece/include/Rook.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool Rook::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    auto canRookMove = [&]() {
        int dRow = toPosition.row - GetPosition().row;
        int dCol = toPosition.col - GetPosition().col;

        if (dRow != 0 && dCol != 0) {
            return false;
        }

        // clang-format off
        constexpr int dr[] = {0,  1,  0,  -1, };
        constexpr int dc[] = {-1, 0,  1,  0, };
        // clang-format on

        // Normalize the change in row and column to -1, 0, or 1
        if (dRow != 0) {
            dRow /= std::abs(dRow);
        }
        if (dCol != 0) {
            dCol /= std::abs(dCol);
        }
        // Search for the direction in the directional arrays
        int index = 0;
        for (; index < 4; ++index) {
            if (dr[index] == dRow && dc[index] == dCol) {
                break;
            }
        }

        // Iterate to the destination, and check if there is any obstacle
        // support king check also
        Position p{GetPosition().row + dr[index], GetPosition().col + dc[index]};
        while (p.IsValid()) {
            if (!std::holds_alternative<EmptyPiece>(square_->GetPieces()[p.row][p.col])) {
                if (validateKingCheck != std::nullopt) {
                    if (!(validateKingCheck.value().row == p.row && validateKingCheck.value().col == p.col)) {
                        break;
                    }
                } else {
                    break;
                }
            }
            if (p.row == toPosition.row && p.col == toPosition.col) {
                break;
            }
            p.row = p.row + dr[index];
            p.col = p.col + dc[index];
        }
        if (p.row != toPosition.row || p.col != toPosition.col) {
            return false;
        }
        return true;
    };

    return canRookMove();
}

bool Rook::IsValidAttackMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidBasicMove_(toPosition, validateKingCheck);
}
} // namespace mlp_ha
