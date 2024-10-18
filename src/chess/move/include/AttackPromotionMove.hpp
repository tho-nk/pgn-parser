#pragma once

#include "move/include/AttackMove.hpp"
#include "move/include/PromotionMove.hpp"

namespace mlp_ha {

class AttackPromotionMoveData : public MoveData {
  public:
    AttackPromotionMoveData(const Color &color) : MoveData(color), fromPosition_(), toPosition_(), promotionType_() {}
    AttackPromotionMoveData(const AttackPromotionMoveData &) = delete;
    AttackPromotionMoveData &operator=(const AttackPromotionMoveData &) = delete;
    AttackPromotionMoveData(AttackPromotionMoveData &&) = delete;
    AttackPromotionMoveData &operator=(AttackPromotionMoveData &&) = delete;
    ~AttackPromotionMoveData() = default;

    virtual void SetFromPosition(const FromPosition &p) override { fromPosition_ = p; }
    virtual const FromPosition &GetFromPosition() const override { return fromPosition_; }

    virtual void SetToPosition(const ToPosition &p) override { toPosition_ = p; }
    virtual const ToPosition &GetToPosition() const override { return toPosition_; }

    virtual void SetPieceType(const PieceType &p) override { pieceType_ = p; }
    virtual const PieceType &GetPieceType() const override { return pieceType_; }

    virtual void SetPromotionType(const PieceType &p) override { promotionType_ = p; }
    virtual const PieceType &GetPromotionType() const override { return promotionType_; }

  private:
    FromPosition fromPosition_;
    ToPosition toPosition_;
    PieceType pieceType_;
    PieceType promotionType_;
};

class AttackPromotionMove : public AttackMove, public PromotionMove {

  public:
    AttackPromotionMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    AttackPromotionMove(const AttackPromotionMove &) = delete;
    AttackPromotionMove &operator=(const AttackPromotionMove &) = delete;
    AttackPromotionMove(AttackPromotionMove &&) = delete;
    AttackPromotionMove &operator=(AttackPromotionMove &&) = delete;
    ~AttackPromotionMove() = default;

    virtual void ProcessMove() final;

  private:
    void ComputeMoveData();
};
} // namespace mlp_ha
