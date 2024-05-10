#pragma once

#include "move/include/Round.hpp"
#include "piece/include/Pieces.hpp"
#include <filesystem>
#include <optional>
#include <variant>

namespace mlp_ha {

class BoardGame : public std::enable_shared_from_this<BoardGame> {
  public:
    BoardGame(const std::filesystem::path &filePath);
    ~BoardGame() = default;

    void LoadData();
    void Draw();
    void Run();

    // Visit BasicMove
    void ProcessBasicMove(const PiecesReference &subPieces, const ToPosition &toPosition, FromPosition &fromPosition);
    void MovePiece(const FromPosition &fromPosition, const ToPosition toPosition);
    bool IsValidMove(const King &king, const ToPosition &toPosition);
    bool IsValidMove(const Queen &queen, const ToPosition &toPosition);
    bool IsValidMove(const Rook &rook, const ToPosition &toPosition);
    bool IsValidMove(const Bishop &bishop, const ToPosition &toPosition);
    bool IsValidMove(const Knight &knight, const ToPosition &toPosition);
    bool IsValidMove(const Pawn &pawn, const ToPosition &toPosition);
    bool IsValidMove(const EmptyPiece &empty, const ToPosition &toPosition);

    // Visit AttackMove
    void ProcessAttackMove(const PiecesReference &subPieces, const ToPosition &toPosition, FromPosition &fromPosition);
    void AttackPiece(const FromPosition &fromPosition, const ToPosition toPosition);
    bool IsValidAttackMove(const King &king, const ToPosition &toPosition);
    bool IsValidAttackMove(const Queen &queen, const ToPosition &toPosition);
    bool IsValidAttackMove(const Rook &rook, const ToPosition &toPosition);
    bool IsValidAttackMove(const Bishop &bishop, const ToPosition &toPosition);
    bool IsValidAttackMove(const Knight &knight, const ToPosition &toPosition);
    bool IsValidAttackMove(const Pawn &pawn, const ToPosition &toPosition);
    bool IsValidAttackMove(const EmptyPiece &empty, const ToPosition &toPosition);

    void ProcessPromotionMove(const PieceType &pieceType, const Color &color, const FromPosition &fromPosition,
                              const ToPosition &toPosition);

    const Pieces &GetPieces() const { return pieces_; }

    void SetEnPassant(const std::optional<Position> &enPassant) { enPassant_ = enPassant; }
    void ResetEnPassant() { enPassant_ = std::nullopt; }

  private:
    // TODO
    bool IsKingChedked() { return false; }

  private:
    std::filesystem::path filePath_;
    Rounds rounds_;
    Pieces pieces_;
    Position whiteKingPosition_;
    Position blackKingPosition_;

    std::optional<Position> enPassant_;
};

} // namespace mlp_ha
