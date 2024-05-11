#include "piece/include/Rook.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool Rook::IsValidBasicMove_(std::shared_ptr<Square> square, const Position &toPosition) const {
    auto canRookMove = [&]() {
        // Rook can move only along rows or columns
        if (GetPosition().row != toPosition.row && GetPosition().col != toPosition.col) {
            return false; // Not moving along a row or column
        }

        if (GetPosition().row == toPosition.row) {
            int start_col = std::min(GetPosition().col, toPosition.col);
            int end_col = std::max(GetPosition().col, toPosition.col);
            for (int c = start_col + 1; c < end_col; ++c) {
                if (!std::holds_alternative<EmptyPiece>(square->GetPieces()[toPosition.row][c])) {
                    return false;
                }
            }
        } else {
            int start_row = std::min(GetPosition().row, GetPosition().row);
            int end_row = std::max(GetPosition().row, GetPosition().row);
            for (int r = start_row + 1; r < end_row; ++r) {
                if (!std::holds_alternative<EmptyPiece>(square->GetPieces()[r][toPosition.col])) {
                    return false;
                }
            }
        }
        return true; // No pieces obstruct movement
    };
    if (canRookMove()) {
        // TODO
        // if (IsKingChedked()) {
        //     return false;
        // }
        return true;
    }
    return false;
}

bool Rook::IsValidAttackMove_(std::shared_ptr<Square> square, const Position &toPosition) const {
    return IsValidBasicMove_(square, toPosition);
}
} // namespace mlp_ha
