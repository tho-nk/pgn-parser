#include "piece/include/Knight.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool Knight::IsValidMove_(const Position &toPosition) const {
    // Possible moves for a knight
    constexpr int dr[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    constexpr int dc[] = {-1, -2, -2, -1, 1, 2, 2, 1};

    // Check all possible moves from knight
    for (int i = 0; i < 8; ++i) {
        Position p{GetPosition().row + dr[i], GetPosition().col + dc[i]};
        // Check if the move leads to toPosition
        if (p == toPosition) {
            return true;
        }
    }
    return false;
}

bool Knight::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition);
}

bool Knight::IsValidAttackMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition);
}
} // namespace mlp_ha
