#pragma once

#include "piece/include/Pieces.hpp"

#include <optional>

namespace mlp_ha {
class Square {
  public:
    Square(/* args */);
    ~Square();

    void InitSquare();
    void Draw();

    const Pieces &GetPieces() const { return pieces_; }

    void SetEnPassant(const std::optional<Position> &enPassant) { enPassant_ = enPassant; }
    void ResetEnPassant() { enPassant_ = std::nullopt; }

    // Visit BasicMove
    void ProcessBasicMove(const PiecesReference &subPieces, const ToPosition &toPosition, FromPosition &fromPosition);
    void MovePiece(const FromPosition &fromPosition, const ToPosition toPosition);

    PiecesReference GetPieceOfTypeAndColor(const PieceType &pieceType, const Color &color,
                                           const FromPosition &fromPosition);

  private:
    bool IsValidMove(const King &king, const ToPosition &toPosition);
    bool IsValidMove(const Queen &queen, const ToPosition &toPosition);
    bool IsValidMove(const Rook &rook, const ToPosition &toPosition);
    bool IsValidMove(const Bishop &bishop, const ToPosition &toPosition);
    bool IsValidMove(const Knight &knight, const ToPosition &toPosition);
    bool IsValidMove(const Pawn &pawn, const ToPosition &toPosition);
    bool IsValidMove(const EmptyPiece &empty, const ToPosition &toPosition);

  public:
    // Visit AttackMove
    void ProcessAttackMove(const PiecesReference &subPieces, const ToPosition &toPosition, FromPosition &fromPosition);
    void AttackPiece(const FromPosition &fromPosition, const ToPosition toPosition);

  private:
    bool IsValidAttackMove(const King &king, const ToPosition &toPosition);
    bool IsValidAttackMove(const Queen &queen, const ToPosition &toPosition);
    bool IsValidAttackMove(const Rook &rook, const ToPosition &toPosition);
    bool IsValidAttackMove(const Bishop &bishop, const ToPosition &toPosition);
    bool IsValidAttackMove(const Knight &knight, const ToPosition &toPosition);
    bool IsValidAttackMove(const Pawn &pawn, const ToPosition &toPosition);
    bool IsValidAttackMove(const EmptyPiece &empty, const ToPosition &toPosition);

  public:
    void ProcessPromotionMove(const PieceType &pieceType, const Color &color, const FromPosition &fromPosition,
                              const ToPosition &toPosition);

  private:
    bool IsKingChedked() { return false; }
    std::optional<Position> enPassant_;
    Pieces pieces_;

    Position whiteKingPosition_;
    Position blackKingPosition_;

    // void doSQuare(const PiecesReference &subPieces, const ToPosition &toPosition) {
    //     for (auto &it : subPieces) {
    //         std::visit(
    //             [&](auto &&piece) {
    //                 isValid = IsValidMove(piece, toPosition);
    //                 square.doQuare();
    //                 if (isValid) {
    //                     if ((fromPosition.row != -1 && fromPosition.row != piece.GetPosition().row) ||
    //                         (fromPosition.col != -1 && fromPosition.col != piece.GetPosition().col)) {
    //                         isValid = false;
    //                     } else {
    //                         fromPosition.row = piece.GetPosition().row;
    //                         fromPosition.col = piece.GetPosition().col;
    //                     }
    //                 }
    //             },
    //             it.get());
    //         if (isValid)
    //             break;
    //     }
    // }
};
} // namespace mlp_ha