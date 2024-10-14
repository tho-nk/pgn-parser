#pragma once

#include "move/include/Round.hpp"
#include "piece/include/Pieces.hpp"

#include <filesystem>
#include <optional>

namespace mlp_ha {
class Square {
  public:
    static Square &GetInstance() {
        static Square instance;
        return instance;
    }

    void LoadPGN(const std::filesystem::path &filePath);

    void Run();

    std::string GetCurrentState() const noexcept;

    const Pieces &GetPieces() const { return pieces_; }
    bool IsEmptyAt(const Position &position) const {
        return std::holds_alternative<EmptyPiece>(pieces_[position.row][position.col]);
    }

    void SetEnPassant(const std::optional<Position> &enPassant) { enPassant_ = enPassant; }
    void ResetEnPassant() { enPassant_ = std::nullopt; }

    PiecesReference GetPieceOfTypeAndColor(const PieceType &pieceType, const Color &color,
                                           const FromPosition &fromPosition) const noexcept;
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
    void ProcessPromotionMove(const PieceType &promotionType, const Color &color, const FromPosition &fromPosition,
                              const ToPosition &toPosition);

  private:
    Square() { Init(); }
    void Init();

    Square(const Square &) = delete;
    Square &operator=(const Square &) = delete;
    Square(Square &&) = default;
    Square &operator=(Square &&) = default;
    ~Square() = default;

    std::optional<Position> enPassant_{std::nullopt};
    Pieces pieces_;
    Rounds rounds_;

  private:
    void ValidateMove(const Position &kingPosition, const Position &piecePosition, const ToPosition &toPosition,
                      const Color &pieceColor, bool &isValid, FromPosition &fromPosition);
    bool VerifyIfKingBeingCheck(const Position &piecePosition, const Color &pieceColor, const Position &kingPosition);
};
} // namespace mlp_ha