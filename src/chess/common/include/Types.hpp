#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace mlp_ha {

constexpr size_t COLUMNS = 8;
constexpr size_t ROWS = 8;

enum class Color { White, Black, Undefined };
std::ostream &operator<<(std::ostream &os, const Color color);

struct Position {
    Position(int r = -1, int c = -1) : row(r), col(c) {}
    int row;
    int col;
    bool IsValid() const { return row >= 0 && row < ROWS && col >= 0 && col < COLUMNS; }

    bool operator==(const Position &other) const { return row == other.row && col == other.col; }
    bool operator!=(const Position &other) const { return !(*this == other); }
};
bool AreOnFileOrRowOrDiagonal(const Position &p1, const Position &p2, const Position p3);

bool AreOnFileOrRowOrDiagonal(const Position &p1, const Position &p2);

enum class PieceType { King, Queen, Rook, Bishop, Knight, Pawn, Undefined };
std::ostream &operator<<(std::ostream &os, const PieceType pieceType);
PieceType StringToPieceType(std::string view);

enum class MoveType { BasicMove, AttackMove, PromotionMove, CastlingMove, AttackPromotionMove, Undefined };
std::ostream &operator<<(std::ostream &os, const MoveType moveType);

using FromPosition = Position;
using ToPosition = Position;
using Positions = std::vector<Position>;

} // namespace mlp_ha
