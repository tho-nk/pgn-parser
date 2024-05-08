#pragma once

// #include "BoardGame.hpp"
#include "Move.hpp"
#include <list>

namespace mlp_ha {
class Round {
  public:
    Round(/*const BoardGame &boardGame,*/ const std::string &str);
    ~Round() = default;

  private:
    int roundIndex_;
    std::string roundComment_;
    // std::tuple<Move, Move> moves_;
    Move aMove;
    // const BoardGame &boardGame_;

  private:
    void ParseRoundText(const std::string &str);
};

using Rounds = std::list<Round>;

} // namespace mlp_ha
