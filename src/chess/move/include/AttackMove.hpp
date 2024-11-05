#pragma once

#include "move/include/Move.hpp"

namespace pgn {

class AttackMove : public virtual Move {

  public:
    AttackMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    AttackMove() = default;
    AttackMove(const AttackMove &) = delete;
    AttackMove &operator=(const AttackMove &) = delete;
    AttackMove(AttackMove &&) = delete;
    AttackMove &operator=(AttackMove &&) = delete;
    ~AttackMove() = default;

    virtual void ProcessMove() override;

  private:
    void ComputeMoveData();
};
} // namespace pgn
