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

namespace mlp_ha {

using Piece = std::variant<EmptyPiece, Bishop, King, Knight, Pawn, Queen, Rook>;
using Pieces = std::array<std::array<Piece, COLUMNS>, ROWS>;
using PiecesReference = std::vector<std::reference_wrapper<const Piece>>;

inline Piece CreatePiece(const std::string &pieceType, const Color &color, const Position &position) {
    const auto type = StringToPieceType(pieceType);

    switch (type) {
    case PieceType::King:
        return King(color, position);
    case PieceType::Queen:
        return Queen(color, position);
    case PieceType::Rook:
        return Rook(color, position);
    case PieceType::Bishop:
        return Bishop(color, position);
    case PieceType::Knight:
        return Knight(color, position);
    case PieceType::Pawn:
        return Pawn(color, position);
    case PieceType::Undefined:
        return EmptyPiece(color, position);
    default:
        std::cerr << "error type" << std::endl;
        break;
    }
}
} // namespace mlp_ha