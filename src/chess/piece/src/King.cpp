#include "piece/include/King.hpp"
#include "piece/include/Square.hpp"

namespace pgn {

bool King::IsValidMove_(const Position &toPosition) const {
    constexpr int dr[] = {-1, 0, 1, 1, 1, 0, -1, -1};
    constexpr int dc[] = {-1, -1, -1, 0, 1, 1, 1, 0};

    for (int index = 0; index < 8; ++index) {
        Position p{GetPosition().row + dr[index], GetPosition().col + dc[index]};
        // Check if the move leads to toPosition
        if (p == toPosition) {
            return true;
        }
    }
    return false;
}

bool King::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition);
}

bool King::IsValidAttackMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidMove_(toPosition);
}
} // namespace pgn
