#pragma once

#include "move/include/Move.hpp"

namespace mlp_ha {

class PromotionMoveData : public MoveData {
  public:
    PromotionMoveData(const Color &color) : MoveData(color), fromPosition_(), toPosition_(), promotionType_() {}
    PromotionMoveData(const PromotionMoveData &) = delete;
    PromotionMoveData &operator=(const PromotionMoveData &) = delete;
    PromotionMoveData(PromotionMoveData &&) = delete;
    PromotionMoveData &operator=(PromotionMoveData &&) = delete;
    ~PromotionMoveData() = default;

    virtual void SetFromPosition(const FromPosition &p) override { fromPosition_ = p; }
    virtual const FromPosition &GetFromPosition() const override { return fromPosition_; }

    virtual void SetToPosition(const ToPosition &p) override { toPosition_ = p; }
    virtual const ToPosition &GetToPosition() const override { return toPosition_; }

    virtual void SetPromotionType(const PieceType &p) override { promotionType_ = p; }
    virtual const PieceType &GetPromotionType() const override { return promotionType_; }

  private:
    FromPosition fromPosition_;
    ToPosition toPosition_;
    PieceType promotionType_;
};

class PromotionMove : public virtual Move {
  public:
    PromotionMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    PromotionMove() = default;
    PromotionMove(const PromotionMove &) = delete;
    PromotionMove &operator=(const PromotionMove &) = delete;
    PromotionMove(PromotionMove &&) = delete;
    PromotionMove &operator=(PromotionMove &&) = delete;
    ~PromotionMove() = default;

    virtual void ProcessMove() override;

  private:
    void ComputeMoveData();
};
} // namespace mlp_ha
