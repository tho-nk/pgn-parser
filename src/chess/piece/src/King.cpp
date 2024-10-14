#include "piece/include/King.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool King::IsValidMove_(const Position &toPosition) const {
    constexpr int dr[] = {-1, 0, 1, 1, 1, 0, -1, -1};
    constexpr int dc[] = {-1, -1, -1, 0, 1, 1, 1, 0};

    for (int index = 0; index < 8; ++index) {
        if (GetPosition().row + dr[index] == toPosition.row && GetPosition().col + dc[index] == toPosition.col) {
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
} // namespace mlp_ha
