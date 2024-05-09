#include "PromotionMove.hpp"

namespace mlp_ha {

PromotionMove::PromotionMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {}

void PromotionMove::ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {}

} // namespace mlp_ha
