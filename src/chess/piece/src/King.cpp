#include "piece/include/King.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool King::IsValidBasicMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                             const std::optional<Position> &validateKingCheck) const {
    auto canKingMove = [&]() {
        constexpr int dr[] = {-1, 0, 1, 1, 1, 0, -1, -1};
        constexpr int dc[] = {-1, -1, -1, 0, 1, 1, 1, 0};

        for (int index = 0; index < 8; ++index) {
            if (GetPosition().row + dr[index] == toPosition.row && GetPosition().col + dc[index] == toPosition.col) {
                return true;
            }
        }
        return false;
    };

    return canKingMove();
}

bool King::IsValidAttackMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                              const std::optional<Position> &validateKingCheck) const {
    return IsValidBasicMove_(square, toPosition, validateKingCheck);
}
} // namespace mlp_ha
