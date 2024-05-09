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

  private:
    Piece GetPiece(const Move &move);

  private:
    std::filesystem::path filePath_;
    Rounds rounds_;
    Pieces pieces_;
};

} // namespace mlp_ha
