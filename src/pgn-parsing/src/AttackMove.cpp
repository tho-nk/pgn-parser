#include "AttackMove.hpp"

namespace mlp_ha {

void AttackMove::ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {}

AttackMove::AttackMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {}
} // namespace mlp_ha
