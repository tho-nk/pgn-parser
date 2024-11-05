#pragma once

#include "move/include/Move.hpp"
#include <iostream>
#include <memory>
#include <vector>

namespace pgn {

class Round {
  public:
    Round(const std::string &str);
    Round(const Round &) = delete;
    Round &operator=(const Round &) = delete;
    Round(Round &&) = default;
    Round &operator=(Round &&) = default;
    ~Round() = default;

    int GetRoundIndex() const { return roundIndex_; }
    const std::string &GetComment() const { return roundComment_; }
    void Run() const;

  private:
    int roundIndex_;
    std::string roundComment_;
    std::unique_ptr<Move> whiteMove_;
    std::unique_ptr<Move> blackMove_;

  private:
    void ParseRoundText(const std::string &str);
};

using Rounds = std::vector<Round>;

} // namespace pgn
