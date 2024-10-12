#pragma once

#include "move/include/Move.hpp"

namespace mlp_ha {

class AttackPromotionMove : public Move {

  public:
    AttackPromotionMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment);
    AttackPromotionMove(const AttackPromotionMove &) = delete;
    AttackPromotionMove &operator=(const AttackPromotionMove &) = delete;
    AttackPromotionMove(AttackPromotionMove &&) = delete;
    AttackPromotionMove &operator=(AttackPromotionMove &&) = delete;
    ~AttackPromotionMove() = default;

    virtual void ProcessMove(Square *square) final;
};
} // namespace mlp_ha
