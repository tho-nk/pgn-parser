#pragma once

#include "move/include/MoveBase.hpp"

namespace pgn {

class AttackMove : public virtual MoveBase {

  public:
    AttackMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    AttackMove() = default;
    AttackMove(const AttackMove &) = delete;
    AttackMove &operator=(const AttackMove &) = delete;
    AttackMove(AttackMove &&) = default;
    AttackMove &operator=(AttackMove &&other) {
        if (this != &other) {
            MoveBase::operator=(std::move(other));
            // Move other members if any
        }
        return *this;
    }
    ~AttackMove() = default;

    void ProcessMove() const;

  private:
    void ComputeMoveData();
};
} // namespace pgn
