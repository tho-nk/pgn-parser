#pragma once

#include "piece/include/Pieces.hpp"

#include <optional>

namespace mlp_ha {
class Square : public std::enable_shared_from_this<Square> {
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
                                           const FromPosition &fromPosition) const;
    Position GetKingPosition(const Color &color) const;

  public:
    // Visit AttackMove
    void ProcessAttackMove(const PiecesReference &subPieces, const ToPosition &toPosition, FromPosition &fromPosition);
    void AttackPiece(const FromPosition &fromPosition, const ToPosition toPosition);

  public:
    void ProcessPromotionMove(const PieceType &pieceType, const Color &color, const FromPosition &fromPosition,
                              const ToPosition &toPosition);

  private:
    bool IsKingChedked() { return false; }
    std::optional<Position> enPassant_;
    Pieces pieces_;

    Position whiteKingPosition_;
    Position blackKingPosition_;

    bool VerifyIfKingBeingCheck(const Position &piecePosition, const Color &pieceColor, const Position &kingPosition);

    std::string GetDraw() const;
};
} // namespace mlp_ha