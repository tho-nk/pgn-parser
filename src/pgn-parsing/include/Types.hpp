#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace mlp_ha {

enum class Color { White, Black, Undefined };
std::ostream &operator<<(std::ostream &os, const Color color);

struct Position {
    int row;
    int col;
};

enum class PieceType { King, Queen, Rook, Bishop, Knight, Pawn, Undefined };
std::ostream &operator<<(std::ostream &os, const PieceType pieceType);
PieceType StringToPieceType(std::string view);

enum class MoveType { BasicMove, AttackMove, PromotionMove, CastlingMove, Undefined };
std::ostream &operator<<(std::ostream &os, const MoveType moveType);

using FromPosition = Position;
using ToPosition = Position;
using Positions = std::vector<Position>;
// using PieceType = std::string;

} // namespace mlp_ha
