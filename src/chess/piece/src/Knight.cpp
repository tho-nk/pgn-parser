#include "piece/include/Knight.hpp"
#include "piece/include/Square.hpp"

namespace pgn {

bool Knight::IsValidMove_(const Position &toPosition) const {
    // Possible moves for a knight
    constexpr int dr[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    constexpr int dc[] = {-1, -2, -2, -1, 1, 2, 2, 1};

    // Check all possible moves from knight
    for (int index = 0; index < 8; ++index) {
        Position p{GetPosition().row + dr[index], GetPosition().col + dc[index]};
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
} // namespace pgn
