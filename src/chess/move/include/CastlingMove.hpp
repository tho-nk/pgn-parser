#pragma once

#include "move/include/MoveBase.hpp"

namespace pgn {

class CastlingMove : public MoveBase {
  public:
    CastlingMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    CastlingMove(const CastlingMove &) = delete;
    CastlingMove &operator=(const CastlingMove &) = delete;
    CastlingMove(CastlingMove &&) = default;
    CastlingMove &operator=(CastlingMove &&) = default;
    ~CastlingMove() = default;

    void ProcessMove() const;

  private:
    void ComputeMoveData();
};
} // namespace pgn
