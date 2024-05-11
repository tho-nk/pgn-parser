#include "piece/include/Bishop.hpp"
#include "piece/include/Square.hpp"
namespace mlp_ha {

bool Bishop::IsValidBasicMove_(std::shared_ptr<Square> square, const Position &toPosition) const {
    auto canBishopMove = [&]() {
        // Calculate the absolute differences in row and column
        int dR = std::abs(GetPosition().row - toPosition.row);
        int dC = std::abs(GetPosition().col - toPosition.col);

        // A bishop can move only diagonally, so the dR and dC must be equal
        // and the positions must not be the same:  && p1 != p2; not need here (the move was confirmed)
        // there is only on bishop can move to that position, no need to check obstacle
        return (dR == dC);
    };
    if (canBishopMove()) {
        // TODO
        // if (IsKingChedked()) {
        //     return false;
        // }
        return true;
    }
    return false;
}

bool Bishop::IsValidAttackMove_(std::shared_ptr<Square> square, const Position &toPosition) const {
    return IsValidBasicMove_(square, toPosition);
}
} // namespace mlp_ha
