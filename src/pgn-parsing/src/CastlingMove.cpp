#include "CastlingMove.hpp"

namespace mlp_ha {

CastlingMove::CastlingMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {}

void CastlingMove::ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {}

} // namespace mlp_ha
