#pragma once

#include "move/include/Move.hpp"

namespace mlp_ha {

class AttackMoveData : public MoveData {
  public:
    AttackMoveData(const Color &color) : MoveData(color), fromPosition_(), toPosition_(), pieceType_() {}
    AttackMoveData(const AttackMoveData &) = delete;
    AttackMoveData &operator=(const AttackMoveData &) = delete;
    AttackMoveData(AttackMoveData &&) = delete;
    AttackMoveData &operator=(AttackMoveData &&) = delete;
    ~AttackMoveData() = default;

    virtual void SetFromPosition(const FromPosition &p) override { fromPosition_ = p; }
    virtual const FromPosition &GetFromPosition() const override { return fromPosition_; }

    virtual void SetToPosition(const ToPosition &p) override { toPosition_ = p; }
    virtual const ToPosition &GetToPosition() const override { return toPosition_; }

    virtual void SetPieceType(const PieceType &p) override { pieceType_ = p; }
    virtual const PieceType &GetPieceType() const override { return pieceType_; }

  private:
    FromPosition fromPosition_;
    ToPosition toPosition_;
    PieceType pieceType_;
};

class AttackMove : public virtual Move {

  public:
    AttackMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    AttackMove() = default;
    AttackMove(const AttackMove &) = delete;
    AttackMove &operator=(const AttackMove &) = delete;
    AttackMove(AttackMove &&) = delete;
    AttackMove &operator=(AttackMove &&) = delete;
    ~AttackMove() = default;

    virtual void ProcessMove() override;

  private:
    void ComputeMoveData();
};
} // namespace mlp_ha
