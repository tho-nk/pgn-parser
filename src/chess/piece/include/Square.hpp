#pragma once

#include "move/include/Round.hpp"
#include "piece/include/Pieces.hpp"

#include <filesystem>
#include <optional>

namespace mlp_ha {
class Square {
  public:
    Square(const std::filesystem::path &filePath) : filePath_(filePath), enPassant_(std::nullopt){};
    Square(const Square &) = delete;
    Square &operator=(const Square &) = delete;
    Square(Square &&) = default;
    Square &operator=(Square &&) = default;
    ~Square() = default;

    void Init();
    std::string GetCurrentState() const;

    const Pieces &GetPieces() const { return pieces_; }

    void SetEnPassant(const std::optional<Position> &enPassant) { enPassant_ = enPassant; }
    void ResetEnPassant() { enPassant_ = std::nullopt; }

    PiecesReference GetPieceOfTypeAndColor(const PieceType &pieceType, const Color &color,
                                           const FromPosition &fromPosition) const;
    Position GetKingPosition(const Color &color) const;

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

    // Process AttackPromotionMove Pawn only
    void ProcessAttackPromotionMove(const PieceType &pieceType, const Color &color, FromPosition &fromPosition,
                                    const ToPosition &toPosition);

    void Run();
    void LoadPGN();

  private:
    std::optional<Position> enPassant_;
    Pieces pieces_;
    Rounds rounds_;
    std::filesystem::path filePath_;

  private:
    void ValidateMove(const Position &kingPosition, const Position &piecePosition, const ToPosition &toPosition,
                      const Color &pieceColor, bool &isValid, FromPosition &fromPosition);
    bool VerifyIfKingBeingCheck(const Position &piecePosition, const Color &pieceColor, const Position &kingPosition);
};
} // namespace mlp_ha