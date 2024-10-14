#include "piece/include/Pawn.hpp"

#include "piece/include/Square.hpp"
namespace mlp_ha {

bool Pawn::IsValidBasicMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    if (GetColor() == Color::White) {
        // no need to check Check. there only one pawn can move
        if ((GetPosition().col != toPosition.col) || (GetPosition().row >= toPosition.row)) {
            return false;
        }
        // one step move
        if (GetPosition().row == (toPosition.row - 1)) {
            return true;
        }
        // has obstacle
        for (int r = GetPosition().row + 1; r < toPosition.row; ++r) {
            if (!mlp_ha::Square::GetInstance().IsEmptyAt({r, toPosition.col})) {
                return false;
            }
        }
        return true;
    } else if (GetColor() == Color::Black) {
        // no need to check Check. there only one pawn can move
        if ((GetPosition().col != toPosition.col) || (GetPosition().row <= toPosition.row)) {
            return false;
        }
        // on step move
        if (GetPosition().row == (toPosition.row + 1)) {
            return true;
        }
        // has obstacle
        for (int r = GetPosition().row - 1; r > toPosition.row; --r) {
            if (!mlp_ha::Square::GetInstance().IsEmptyAt({r, toPosition.col})) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool Pawn::IsValidAttackMove_(const Position &toPosition, const std::optional<Position> &validateKingCheck) const {
    if (GetColor() == Color::White) {
        // en passant
        if (mlp_ha::Square::GetInstance().IsEmptyAt(toPosition)) {
            mlp_ha::Square::GetInstance().SetEnPassant(Position{toPosition.row - 1, toPosition.col});
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
        if (mlp_ha::Square::GetInstance().IsEmptyAt(toPosition)) {
            mlp_ha::Square::GetInstance().SetEnPassant(Position{toPosition.row + 1, toPosition.col});
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
