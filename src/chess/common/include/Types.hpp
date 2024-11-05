#pragma once

#include <iostream>
#include <string>

namespace pgn {

constexpr size_t COLUMNS = 8;
constexpr size_t ROWS = 8;

enum class Color { White, Black, Undefined };
std::ostream &operator<<(std::ostream &os, const Color color);

struct Position {
    Position(int r = -1, int c = -1) : row(r), col(c) {}
    Position(const Position &other) : row(other.row), col(other.col) {}
    Position &operator=(const Position &other) {
        if (this != &other) {
            row = other.row;
            col = other.col;
        }
        return *this;
    }

    bool operator==(const Position &other) const { return row == other.row && col == other.col; }
    bool operator!=(const Position &other) const { return !(*this == other); }
    Position &operator+=(const Position &other) {
        row += other.row;
        col += other.col;
        return *this;
    }
    Position operator+(const Position &other) const { return Position(row + other.row, col + other.col); }
    void Shift(int dr, int dc) {
        row += dr;
        col += dc;
    }
    bool IsValid() const { return row >= 0 && row < ROWS && col >= 0 && col < COLUMNS; }

    void Reset() {
        row = -1;
        col = -1;
    }

    int row;
    int col;
};

inline bool AreOnFile(const Position &p1, const Position &p2) { return p1.col == p2.col; }
inline bool AreOnRow(const Position &p1, const Position &p2) { return p1.row == p2.row; }
inline bool AreOnDiagonal(const Position &p1, const Position &p2) {
    return std::abs(p1.row - p2.row) == std::abs(p1.col - p2.col);
}
bool AreOnFileOrRowOrDiagonal(const Position &p1, const Position &p2, const Position p3);
bool AreOnFileOrRowOrDiagonal(const Position &p1, const Position &p2);

enum class PieceType { King, Queen, Rook, Bishop, Knight, Pawn, Undefined };
std::ostream &operator<<(std::ostream &os, const PieceType pieceType);
PieceType StringToPieceType(const std::string &view);

enum class MoveType { BasicMove, AttackMove, PromotionMove, CastlingMove, AttackPromotionMove, Undefined };
std::ostream &operator<<(std::ostream &os, const MoveType moveType);

using FromPosition = Position;
using ToPosition = Position;

/**
 * @brief Data structure that holds the information about a move.
 * This is helpful for moving forward and backward in the game.
 */
struct MoveData {
    Color color;
    Position fromPosition;
    Position toPosition;
    PieceType pieceType = PieceType::Undefined;
    PieceType promotionType = PieceType::Undefined;
    // Castling
    FromPosition kingSource;
    ToPosition kingDesination;
    FromPosition rookSource;
    ToPosition rookDesination;
    MoveData(const Color &color) : color(color) {}
};
} // namespace pgn
