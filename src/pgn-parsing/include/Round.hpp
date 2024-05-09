#pragma once

// #include "BoardGame.hpp"
#include "Move.hpp"
#include <list>

namespace mlp_ha {
class Round {
  public:
    Round(/*const BoardGame &boardGame,*/ const std::string &str);
    ~Round() = default;

    int GetRoundIndex() const { return roundIndex_; }
    const Move &GetWhiteMove() const { return whiteMove_; }
    const Move &GetBlackMove() const { return blackMove_; }
    const std::string &GetComment() const { return roundComment_; }

  private:
    int roundIndex_;
    std::string roundComment_;
    Move whiteMove_;
    Move blackMove_;
    // const BoardGame &boardGame_;

  private:
    void ParseRoundText(const std::string &str);
};

using Rounds = std::list<Round>;

} // namespace mlp_ha
