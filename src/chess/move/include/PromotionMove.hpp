#pragma once

#include "move/include/MoveBase.hpp"

namespace pgn {

class PromotionMove : public virtual MoveBase {
  public:
    PromotionMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    PromotionMove() = default;
    PromotionMove(const PromotionMove &) = delete;
    PromotionMove &operator=(const PromotionMove &) = delete;
    PromotionMove(PromotionMove &&) = default;
    PromotionMove &operator=(PromotionMove &&other) {
        if (this != &other) {
            MoveBase::operator=(std::move(other));
            // Move other members if any
        }
        return *this;
    }
    ~PromotionMove() = default;

    void ProcessMove() const;

  private:
    void ComputeMoveData();
};
} // namespace pgn
