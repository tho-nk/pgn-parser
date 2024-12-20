#pragma once

#include "move/include/AttackMove.hpp"
#include "move/include/PromotionMove.hpp"

namespace pgn {

class AttackPromotionMove : public AttackMove, public PromotionMove {

  public:
    AttackPromotionMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    AttackPromotionMove(const AttackPromotionMove &) = delete;
    AttackPromotionMove &operator=(const AttackPromotionMove &) = delete;
    AttackPromotionMove(AttackPromotionMove &&) = default;
    AttackPromotionMove &operator=(AttackPromotionMove &&) = default;
    ~AttackPromotionMove() = default;

    void ProcessMove() const;

  private:
    void ComputeMoveData();
};
} // namespace pgn
