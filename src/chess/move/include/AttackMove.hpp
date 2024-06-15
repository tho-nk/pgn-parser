#pragma once

#include "move/include/Move.hpp"

namespace mlp_ha {

class AttackMove : public Move {

  public:
    AttackMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment);
    AttackMove(const AttackMove &) = delete;
    AttackMove &operator=(const AttackMove &) = delete;
    AttackMove(AttackMove &&) = delete;
    AttackMove &operator=(AttackMove &&) = delete;
    ~AttackMove() = default;

    virtual void ProcessMove(Square *square) final;
};
} // namespace mlp_ha
