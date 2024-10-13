
#include "common/include/GameHelper.hpp"
#include "piece/include/Square.hpp"
#include <cassert>

namespace mlp_ha {
bool ValidateMove(int dRow, int dCol, const Position &toPosition, const std::optional<Position> &validateKingCheck,
                  const Square *square, Position p) {
    // Assert that dRow and dCol are not both zero, since a move must involve some change in position
    assert(dRow != 0 || dCol != 0);
    if (dRow != 0) {
        dRow /= std::abs(dRow);
    }
    if (dCol != 0) {
        dCol /= std::abs(dCol);
    }

    p.Shift(dRow, dCol);
    while (p.IsValid()) {
        if (p == toPosition) {
            return true;
        }
        if (!square->IsEmpty({p.row, p.col})) {
            if (!validateKingCheck || validateKingCheck.value() != p) {
                return false;
            }
        }
        p.Shift(dRow, dCol);
    }
    return false;
}
} // namespace mlp_ha
