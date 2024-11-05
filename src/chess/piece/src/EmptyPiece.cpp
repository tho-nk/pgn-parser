#include "piece/include/EmptyPiece.hpp"

#include "piece/include/Square.hpp"
namespace pgn {

bool EmptyPiece::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    return false;
}

bool EmptyPiece::IsValidAttackMove_(const Position &toPosition,
                                    const std::optional<Position> &validateKingCheck) const {
    return false;
}
} // namespace pgn
