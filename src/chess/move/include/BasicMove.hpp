#pragma once

#include "move/include/MoveBase.hpp"

namespace pgn {

class BasicMove : public MoveBase {
  public:
    BasicMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    BasicMove(const BasicMove &) = delete;
    BasicMove &operator=(const BasicMove &) = delete;
    BasicMove(BasicMove &&) = default;
    BasicMove &operator=(BasicMove &&) = default;
    ~BasicMove() = default;

    void ProcessMove() const;

  private:
    void ComputeMoveData();
};
} // namespace pgn
