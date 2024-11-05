#pragma once

#include "move/include/Move.hpp"

namespace pgn {

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
} // namespace pgn
