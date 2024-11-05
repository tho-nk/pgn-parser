#pragma once

#include "move/include/AttackMove.hpp"
#include "move/include/PromotionMove.hpp"

namespace pgn {

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
} // namespace pgn
