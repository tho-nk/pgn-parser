#pragma once

#include "piece/include/Bishop.hpp"
#include "piece/include/EmptyPiece.hpp"
#include "piece/include/King.hpp"
#include "piece/include/Knight.hpp"
#include "piece/include/Pawn.hpp"
#include "piece/include/Queen.hpp"
#include "piece/include/Rook.hpp"
#include <array>
#include <variant>

namespace pgn {

using Piece = std::variant<EmptyPiece, Bishop, King, Knight, Pawn, Queen, Rook>;
using Pieces = std::array<std::array<Piece, COLUMNS>, ROWS>;

} // namespace pgn