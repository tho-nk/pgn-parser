#include "piece/include/King.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool King::IsValidBasicMove_(std::shared_ptr<Square> square, const Position &toPosition) const {
    auto canQueenMove = [&]() { return true; }();
    return canQueenMove;
}

bool King::IsValidAttackMove_(std::shared_ptr<Square> square, const Position &toPosition) const {
    return IsValidBasicMove_(square, toPosition);
}
} // namespace mlp_ha
