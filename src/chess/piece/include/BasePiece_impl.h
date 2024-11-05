#include "piece/include/BasePiece.hpp"
#include <cassert>

namespace pgn {

template <typename ConcretePiece>
template <typename IsEmpty>
bool BasePiece<ConcretePiece>::ValidateMove_(int dRow, int dCol, const Position &toPosition,
                                             const std::optional<Position> &validateKingCheck, Position p,
                                             const IsEmpty &isEmpty) const {
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
        if (!isEmpty(p)) {
            if (!validateKingCheck || validateKingCheck.value() != p) {
                return false;
            }
        }
        p.Shift(dRow, dCol);
    }
    return false;
}

} // namespace pgn
