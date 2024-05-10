#pragma once

#include "Pieces.hpp"
#include "Round.hpp"
#include <filesystem>
#include <variant>

namespace mlp_ha {

class BoardGame : public std::enable_shared_from_this<BoardGame> {
  public:
    BoardGame(const std::filesystem::path &filePath);
    ~BoardGame() = default;

    void LoadData();
    void Draw();
    void Run();

    // Visit pieces to validate move
    void ProcessMove(const King &king, const FromPosition &FromPosition);
    void ProcessMove(const Queen &queen, const FromPosition &FromPosition);
    void ProcessMove(const Rook &rook, const FromPosition &FromPosition);
    void ProcessMove(const Bishop &bishop, const FromPosition &FromPosition);
    void ProcessMove(const Knight &knight, const FromPosition &FromPosition);
    void ProcessMove(const Pawn &pawn, const FromPosition &FromPosition);
    void ProcessMove(const EmptyPiece &empty, const FromPosition &FromPosition);

    // Visit pieces to get a from position

    bool IsValidMove(const King &king, const ToPosition &toPosition);
    bool IsValidMove(const Queen &queen, const ToPosition &toPosition);
    bool IsValidMove(const Rook &rook, const ToPosition &toPosition);
    bool IsValidMove(const Bishop &bishop, const ToPosition &toPosition);
    bool IsValidMove(const Knight &knight, const ToPosition &toPosition);
    bool IsValidMove(const Pawn &pawn, const ToPosition &toPosition);
    bool IsValidMove(const EmptyPiece &empty, const ToPosition &toPosition);

    void ComputeFromPosition(const PiecesReference &subPieces, const ToPosition &toPosition,
                             FromPosition &fromPosition);

    void MovePiece(const FromPosition &fromPosition, const ToPosition toPosition);
    const Pieces &GetPieces() const { return pieces_; }

  private:
    // TODO
    bool IsKingChedked() { return false; }

  private:
    std::filesystem::path filePath_;
    Rounds rounds_;
    Pieces pieces_;
    Position whiteKingPosition_;
    Position blackKingPosition_;
};

} // namespace mlp_ha
