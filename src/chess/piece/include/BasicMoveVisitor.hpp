#pragma once

#include "piece/include/Piece.hpp"

namespace mlp_ha {
// helper type for the visitor #4
template <class... Ts> struct BasicMoveVisitor : Ts... {
    using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts> BasicMoveVisitor(Ts...) -> BasicMoveVisitor<Ts...>;
} // namespace mlp_ha
