#pragma once

#include "move/include/AttackMove.hpp"
#include "move/include/PromotionMove.hpp"

namespace mlp_ha {

class AttackPromotionMove : public AttackMove, public PromotionMove {

  public:
    AttackPromotionMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    AttackPromotionMove(const AttackPromotionMove &) = delete;
    AttackPromotionMove &operator=(const AttackPromotionMove &) = delete;
    AttackPromotionMove(AttackPromotionMove &&) = delete;
    AttackPromotionMove &operator=(AttackPromotionMove &&) = delete;
    ~AttackPromotionMove() = default;

    virtual void ProcessMove() final;

    virtual bool PreValidateMove() final;

  private:
    void ComputeMoveData();
};
} // namespace mlp_ha
