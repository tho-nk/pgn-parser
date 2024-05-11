#include "piece/include/Pawn.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool Pawn::IsValidBasicMove_(const std::shared_ptr<Square> &square, const Position &toPosition) const {
    if (GetColor() == Color::White) {
        // no need to check Check. there only one pawn can move
        if ((GetPosition().col != toPosition.col) || (GetPosition().row >= toPosition.row)) {
            return false;
        }
        if (GetPosition().row == (toPosition.row - 1)) {
            return true;
        }
        for (int r = GetPosition().row + 1; r < toPosition.row; ++r) {
            if (!std::holds_alternative<EmptyPiece>(square->GetPieces()[r][toPosition.col])) {
                return false;
            }
        }
        return true;
    } else if (GetColor() == Color::Black) {
        // no need to check Check. there only one pawn can move
        if ((GetPosition().col != toPosition.col) || (GetPosition().row <= toPosition.row)) {
            return false;
        }
        if (GetPosition().row == (toPosition.row + 1)) {
            return true;
        }
        for (int r = GetPosition().row - 1; r > toPosition.row; --r) {
            if (!std::holds_alternative<EmptyPiece>(square->GetPieces()[r][toPosition.col])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool Pawn::IsValidAttackMove_(const std::shared_ptr<Square> &square, const Position &toPosition) const {
    if (GetColor() == Color::White) {
        // en passant
        if (std::holds_alternative<EmptyPiece>(square->GetPieces()[toPosition.row][toPosition.col])) {
            square->SetEnPassant(Position{toPosition.row - 1, toPosition.col});
        }
        // no need to check Check. there only one pawn can move
        if (std::abs(GetPosition().col - toPosition.col) != 1) {
            return false;
        }
        if (GetPosition().row + 1 != toPosition.row) {
            return false;
        }
        return true;
    } else if (GetColor() == Color::Black) {
        if (std::holds_alternative<EmptyPiece>(square->GetPieces()[toPosition.row][toPosition.col])) {
            square->SetEnPassant(Position{toPosition.row + 1, toPosition.col});
        }
        // no need to check Check. there only one pawn can move
        if (std::abs(GetPosition().col - toPosition.col) != 1) {
            return false;
        }
        if (GetPosition().row - 1 != toPosition.row) {
            return false;
        }
        return true;
    }
    return false;
}
} // namespace mlp_ha
