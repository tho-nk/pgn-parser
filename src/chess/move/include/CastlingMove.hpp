#pragma once

#include "move/include/Move.hpp"

namespace pgn {

class CastlingMove : public Move {
  public:
    CastlingMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    CastlingMove(const CastlingMove &) = delete;
    CastlingMove &operator=(const CastlingMove &) = delete;
    CastlingMove(CastlingMove &&) = delete;
    CastlingMove &operator=(CastlingMove &&) = delete;
    ~CastlingMove() = default;

    virtual void ProcessMove() final;

  private:
    void ComputeMoveData();
};
} // namespace pgn
