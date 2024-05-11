#pragma once

#include "move/include/Round.hpp"
#include "piece/include/Pieces.hpp"
#include "piece/include/Square.hpp"
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

    // Visit AttackMove
    void ProcessAttackMove(const PiecesReference &subPieces, const ToPosition &toPosition, FromPosition &fromPosition);
    void AttackPiece(const FromPosition &fromPosition, const ToPosition toPosition);

    void ProcessPromotionMove(const PieceType &pieceType, const Color &color, const FromPosition &fromPosition,
                              const ToPosition &toPosition);

    PiecesReference GetPieceOfTypeAndColor(const PieceType &pieceType, const Color &color,
                                           const FromPosition &fromPosition);

  private:
    // TODO
    bool IsKingChedked() { return false; }

  private:
    std::filesystem::path filePath_;
    Rounds rounds_;
    Square square_;
};

} // namespace mlp_ha
