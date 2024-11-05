#pragma once

#include "move/include/Move.hpp"

namespace pgn {

class BasicMove : public Move {
  public:
    BasicMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    BasicMove(const BasicMove &) = delete;
    BasicMove &operator=(const BasicMove &) = delete;
    BasicMove(BasicMove &&) = delete;
    BasicMove &operator=(BasicMove &&) = delete;
    ~BasicMove() = default;

    virtual void ProcessMove() final;

  private:
    void ComputeMoveData();
};
} // namespace pgn
