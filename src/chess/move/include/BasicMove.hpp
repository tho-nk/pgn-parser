#pragma once

#include "move/include/Move.hpp"

namespace mlp_ha {

class BasicMoveData : public MoveData {
  public:
    BasicMoveData(const Color &color) : MoveData(color), fromPosition_(), toPosition_(), pieceType_() {}
    BasicMoveData(const BasicMoveData &) = delete;
    BasicMoveData &operator=(const BasicMoveData &) = delete;
    BasicMoveData(BasicMoveData &&) = delete;
    BasicMoveData &operator=(BasicMoveData &&) = delete;
    ~BasicMoveData() = default;

    virtual void SetFromPosition(const FromPosition &p) override { fromPosition_ = p; }
    virtual const FromPosition &GetFromPosition() const override { return fromPosition_; }

    virtual void SetToPosition(const ToPosition &p) override { toPosition_ = p; }
    virtual const ToPosition &GetToPosition() const { return toPosition_; }

    virtual void SetPieceType(const PieceType &p) override { pieceType_ = p; }
    virtual const PieceType &GetPieceType() const { return pieceType_; }

  private:
    FromPosition fromPosition_;
    ToPosition toPosition_;
    PieceType pieceType_;
};

class BasicMove : public Move {
  public:
    BasicMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    BasicMove(const BasicMove &) = delete;
    BasicMove &operator=(const BasicMove &) = delete;
    BasicMove(BasicMove &&) = delete;
    BasicMove &operator=(BasicMove &&) = delete;
    ~BasicMove() = default;

    virtual void ProcessMove() final;

  private:
    void ComputeMoveData();
};
} // namespace mlp_ha
