#pragma once

#include "Move.hpp"
#include "Types.hpp"
#include <memory>

namespace mlp_ha {

namespace move_factory {
std::shared_ptr<Move> CreateMove(const MoveType &moveType, const Color &color, std::string moveText,
                                 std::string comment);
}

} // namespace mlp_ha