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
    BoardGame(const BoardGame &) = delete;
    BoardGame &operator=(const BoardGame &) = delete;
    BoardGame(BoardGame &&) = delete;
    BoardGame &operator=(BoardGame &&) = delete;
    ~BoardGame() = default;

    void LoadData();
    void Draw();
    void Run();

    // Process BasicMove
    void ProcessBasicMove(const PiecesReference &subPieces, const Color &color, const ToPosition &toPosition,
                          FromPosition &fromPosition);
    void MovePiece(const FromPosition &fromPosition, const ToPosition toPosition);

    // Process AttackMove
    void ProcessAttackMove(const PiecesReference &subPieces, const Color &color, const ToPosition &toPosition,
                           FromPosition &fromPosition);
    void AttackPiece(const FromPosition &fromPosition, const ToPosition toPosition);

    // Process PromotionMove
    void ProcessPromotionMove(const PieceType &pieceType, const Color &color, const FromPosition &fromPosition,
                              const ToPosition &toPosition);

    PiecesReference GetPieceOfTypeAndColor(const PieceType &pieceType, const Color &color,
                                           const FromPosition &fromPosition);

  private:
    std::filesystem::path filePath_;
    Rounds rounds_;
    std::shared_ptr<Square> square_;
};

} // namespace mlp_ha
