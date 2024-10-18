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

    Square(const Square &) = delete;
    Square &operator=(const Square &) = delete;
    Square(Square &&) = delete;
    Square &operator=(Square &&) = delete;

    void LoadPGN(const std::filesystem::path &filePath);

    void Run();

    std::string GetCurrentState() const noexcept;

    const Pieces &GetPieces() const { return pieces_; }
    Pieces &GetPieces() { return pieces_; }
    const Piece &GetPiecesAt(const Position &position) const noexcept {
        return GetPieces()[position.row][position.col];
    }
    Piece &GetPiecesAt(const Position &position) noexcept { return GetPieces()[position.row][position.col]; }

    bool IsEmptyAt(const Position &position) const {
        return std::holds_alternative<EmptyPiece>(pieces_[position.row][position.col]);
    }

    void SetEnPassant(const Position &enPassant) { enPassant_ = enPassant; }

    PiecesReference GetPieceOfTypeAndColor(const PieceType &pieceType, const Color &color,
                                           const FromPosition &fromPosition) const noexcept;
    Position GetKingPosition(const Color &color) const;

    void ProcessBasicMove(const PiecesReference &subPieces, MoveData &moveData) const;

    void ProcessAttackMove(const PiecesReference &subPieces, MoveData &moveData) const;

    void ProcessPromotionMove(MoveData &moveData);

    void MovePiece(const FromPosition &fromPosition, const ToPosition &toPosition);

    void AttackPiece(const FromPosition &fromPosition, const ToPosition &toPosition);

  private:
    Square() = default;
    ~Square() = default;

    Position enPassant_;
    Pieces pieces_;
    Rounds rounds_;

  private:
    void ResetState_();
    void ValidateMove_(const Position &kingPosition, const Position &piecePosition, MoveData &moveData,
                       bool &isValid) const;
    bool VerifyIfKingBeingCheck_(const Position &piecePosition, const Color &pieceColor,
                                 const Position &kingPosition) const;
};
} // namespace mlp_ha