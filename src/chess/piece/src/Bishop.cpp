#include "piece/include/Bishop.hpp"
#include "piece/include/Square.hpp"
namespace mlp_ha {

bool Bishop::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    auto canBishopMove = [&]() {
        int dRow = toPosition.row - GetPosition().row;
        int dCol = toPosition.col - GetPosition().col;

        // A bishop can move only diagonally, so the dR and dC must be equal
        // and the positions must not be the same:  && p1 != p2; not need here (the move was confirmed)
        // there is only on bishop can move to that position, no need to check obstacle
        // finally obstacle should be checked

        if (std::abs(dRow) != std::abs(dCol)) {
            return false;
        }

        // clang-format off
        constexpr int dr[] = {-1,  1,  1,  -1, };
        constexpr int dc[] = {-1,  -1,  1,  1, };
        // clang-format on

        // Normalize the change in row and column to -1, 0, or 1
        dRow /= std::abs(dRow);
        dCol /= std::abs(dCol);

        int index = 0;
        for (; index < 4; ++index) {
            if (dr[index] == dRow && dc[index] == dCol) {
                break;
            }
        }
        if (index >= 4) {
            std::cerr << "[THO][E] Bishop::IsValidBasicMove_ Cannot find direction" << std::endl;
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

    return canBishopMove();
}

bool Bishop::IsValidAttackMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return IsValidBasicMove_(toPosition, validateKingCheck);
}
} // namespace mlp_ha
