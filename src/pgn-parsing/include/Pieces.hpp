#pragma once

#include "Bishop.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"
#include "Queen.hpp"
#include "Rook.hpp"
#include <array>
#include <variant>

namespace mlp_ha {

class EmptyPiece : public BasePiece<EmptyPiece> {
  public:
    EmptyPiece(const Color &color, const Position &position) {
        type_ = PieceType::Undefined;
        color_ = color;
        position_ = position;
    }

    Positions ComputeFromPositions(const std::string &) { return {}; }

    EmptyPiece() = default;
};

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