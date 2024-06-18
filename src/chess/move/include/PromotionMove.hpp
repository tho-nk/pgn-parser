#pragma once

#include "move/include/Move.hpp"

namespace mlp_ha {

class PromotionMove : public Move {
  public:
    PromotionMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment);
    PromotionMove(const PromotionMove &) = delete;
    PromotionMove &operator=(const PromotionMove &) = delete;
    PromotionMove(PromotionMove &&) = delete;
    PromotionMove &operator=(PromotionMove &&) = delete;
    ~PromotionMove() = default;

    virtual void ProcessMove(const std::shared_ptr<Square> &square) final;
};
} // namespace mlp_ha
