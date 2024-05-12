#include "piece/include/Knight.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool Knight::IsValidBasicMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                               const std::optional<Position> &validateKingCheck) const {
    auto canKnightMove = [&]() {
        // Possible moves for a knight
        constexpr int dr[] = {-2, -1, 1, 2, 2, 1, -1, -2};
        constexpr int dc[] = {-1, -2, -2, -1, 1, 2, 2, 1};

        // Check all possible moves from knight
        for (int i = 0; i < 8; ++i) {
            int newR = GetPosition().row + dr[i];
            int newC = GetPosition().col + dc[i];
            // Check if the move leads to toPosition
            if (newR == toPosition.row && newC == toPosition.col) {
                return true;
            }
        }
        // If none of the moves lead to toPosition
        return false;
    };
    return canKnightMove();
}

bool Knight::IsValidAttackMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                                const std::optional<Position> &validateKingCheck) const {
    return IsValidBasicMove_(square, toPosition, validateKingCheck);
}
} // namespace mlp_ha
