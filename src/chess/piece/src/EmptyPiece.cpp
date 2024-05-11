#include "piece/include/EmptyPiece.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool EmptyPiece::IsValidBasicMove_(std::shared_ptr<Square> square, const Position &toPosition) const { return false; }

bool EmptyPiece::IsValidAttackMove_(std::shared_ptr<Square> square, const Position &toPosition) const {
    return IsValidBasicMove_(square, toPosition);
}
} // namespace mlp_ha
