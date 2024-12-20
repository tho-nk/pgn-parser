#pragma once

#include "move/include/AttackMove.hpp"
#include "move/include/AttackPromotionMove.hpp"
#include "move/include/BasicMove.hpp"
#include "move/include/CastlingMove.hpp"
#include "move/include/MoveBase.hpp"
#include "move/include/PromotionMove.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

namespace pgn {
using Move = std::variant<MoveBase, BasicMove, PromotionMove, CastlingMove, AttackMove, AttackPromotionMove>;

} // namespace pgn
