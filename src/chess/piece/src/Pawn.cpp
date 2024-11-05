#include "piece/include/Pawn.hpp"
#include "piece/include/Square.hpp"

namespace pgn {

bool Pawn::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    int dRow = toPosition.row - GetPosition().row;
    int dCol = toPosition.col - GetPosition().col;

    if (GetColor() == Color::White) {
        // White pawns move up the board (increasing row number)
        if (dCol == 0) {
            // One step forward
            if (dRow == 1 && pgn::Square::GetInstance().IsEmptyAt(toPosition)) {
                return true;
            }
            // Two steps forward from initial position
            if (dRow == 2 && GetPosition().row == 1 && pgn::Square::GetInstance().IsEmptyAt(toPosition) &&
                pgn::Square::GetInstance().IsEmptyAt({GetPosition().row + 1, GetPosition().col})) {
                return true;
            }
        }
    } else if (GetColor() == Color::Black) {
        // Black pawns move down the board (decreasing row number)
        if (dCol == 0) {
            // One step forward
            if (dRow == -1 && pgn::Square::GetInstance().IsEmptyAt(toPosition)) {
                return true;
            }
            // Two steps forward from initial position
            if (dRow == -2 && GetPosition().row == 6 && pgn::Square::GetInstance().IsEmptyAt(toPosition) &&
                pgn::Square::GetInstance().IsEmptyAt({GetPosition().row - 1, GetPosition().col})) {
                return true;
            }
        }
    }
    return false;
}

bool Pawn::IsValidAttackMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    int dRow = toPosition.row - GetPosition().row;
    int dCol = toPosition.col - GetPosition().col;
    if (GetColor() == Color::White) {
        // White pawns move up the board (increasing row number)
        if (std::abs(dCol) == 1 && dRow == 1) {
            // Attack move
            if (!pgn::Square::GetInstance().IsEmptyAt(toPosition)) {
                return true;
            }
            // En passant
            pgn::Square::GetInstance().SetEnPassant({GetPosition().row, toPosition.col});
            return true;
        }
    } else if (GetColor() == Color::Black) {
        // Black pawns move down the board (decreasing row number)
        if (std::abs(dCol) == 1 && dRow == -1) {
            // Diagonal capture
            if (!pgn::Square::GetInstance().IsEmptyAt(toPosition)) {
                return true;
            }
            // En passant
            pgn::Square::GetInstance().SetEnPassant({GetPosition().row, toPosition.col});
            return true;
        }
    }
    return false;
}
} // namespace pgn
