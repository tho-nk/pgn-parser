#pragma once

#include "move/include/Move.hpp"

namespace pgn {

class BasicMove : public Move {
  public:
    BasicMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    BasicMove(const BasicMove &) = delete;
    BasicMove &operator=(const BasicMove &) = delete;
    BasicMove(BasicMove &&) = default;
    BasicMove &operator=(BasicMove &&) = default;
    ~BasicMove() = default;

    void ProcessMove();

  private:
    void ComputeMoveData();
};
} // namespace pgn
