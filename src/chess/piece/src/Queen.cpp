#include "piece/include/Queen.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool Queen::IsValidBasicMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                              const std::optional<Position> &validateKingCheck)
    const { // basiclly there is only one queen, no need to check but for security
    auto canQueenMove = [&]() {
        int dRow = toPosition.row - GetPosition().row;
        int dCol = toPosition.col - GetPosition().col;

        if ((dRow != 0 && dCol != 0) && (std::abs(dRow) != std::abs(dCol))) {
            return false;
        }

        constexpr int dr[] = {-1, 0, 1, 1, 1, 0, -1, -1};
        constexpr int dc[] = {-1, -1, -1, 0, 1, 1, 1, 0};

        if (dRow != 0) {
            dRow /= std::abs(dRow);
        }
        if (dCol != 0) {
            dCol /= std::abs(dCol);
        }

        // Search for the direction in the directional arrays
        int index = 0;
        for (; index < 8; ++index) {
            if (dr[index] == dRow && dc[index] == dCol) {
                break;
            }
        }

        if (index >= 8) {
            // std::cerr << "[THO][E] Queen::IsValidBasicMove_ Cannot find direction" << std::endl;
        }
        Position p{GetPosition().row + dr[index], GetPosition().col + dc[index]};
        while (p.IsValid()) {
            if (!std::holds_alternative<EmptyPiece>(square->GetPieces()[p.row][p.col])) {
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

    return canQueenMove();
}

bool Queen::IsValidAttackMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                               const std::optional<Position> &validateKingCheck) const {
    return IsValidBasicMove_(square, toPosition, validateKingCheck);
}
} // namespace mlp_ha
