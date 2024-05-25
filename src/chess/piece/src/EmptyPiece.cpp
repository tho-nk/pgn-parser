#include "piece/include/EmptyPiece.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool EmptyPiece::IsValidBasicMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                                   const std::optional<Position> &validateKingCheck) const {
    return false;
}

bool EmptyPiece::IsValidAttackMove_(const std::shared_ptr<Square> &square, const Position &toPosition,
                                    const std::optional<Position> &validateKingCheck) const {
    return IsValidBasicMove_(square, toPosition, validateKingCheck);
}
} // namespace mlp_ha
