#pragma once

#include "common/include/Types.hpp"
#include "move/include/AttackMove.hpp"
#include "move/include/AttackPromotionMove.hpp"
#include "move/include/BasicMove.hpp"
#include "move/include/CastlingMove.hpp"
#include "move/include/Move.hpp"
#include "move/include/PromotionMove.hpp"
#include <type_traits>
#include <variant>

namespace pgn {

using MoveVariant =
    std::variant<std::monostate, BasicMove, AttackMove, PromotionMove, CastlingMove, AttackPromotionMove>;

namespace move_factory {
MoveVariant CreateMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
}

inline void ProcessMove(MoveVariant &move) {
    std::visit(
        [](auto &moveObj) {
            using T = std::decay_t<decltype(moveObj)>;
            if constexpr (!std::is_same_v<T, std::monostate>) {
                moveObj.ProcessMove();
            }
        },
        move);
}

} // namespace pgn