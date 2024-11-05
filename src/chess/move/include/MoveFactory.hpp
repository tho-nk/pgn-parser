#pragma once

#include "common/include/Types.hpp"
#include "move/include/Move.hpp"
#include <memory>

namespace pgn {

namespace move_factory {
std::unique_ptr<Move> CreateMove(const MoveType &moveType, const Color &color, std::string &&moveText,
                                 std::string &&comment);
}

} // namespace pgn