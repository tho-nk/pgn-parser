#include "piece/include/Queen.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool Queen::IsValidBasicMove_(std::shared_ptr<Square> square, const Position &toPosition)
    const { // basiclly there is only one queen, no need to check but for security
    auto canQueenMove = [&]() { return true; };
    if (canQueenMove()) {
        // TODO
        // if (IsKingChedked()) {
        //     return false;
        // }
        return true;
    }
    return false;
}

bool Queen::IsValidAttackMove_(std::shared_ptr<Square> square, const Position &toPosition) const {
    return IsValidBasicMove_(square, toPosition);
}
} // namespace mlp_ha
