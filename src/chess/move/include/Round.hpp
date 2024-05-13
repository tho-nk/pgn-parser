#pragma once

// #include "BoardGame.hpp"
#include "move/include/Move.hpp"
#include <memory>
#include <vector>

namespace mlp_ha {
class Round {
  public:
    Round(const std::string &str);
    Round(const Round &) = delete;
    Round &operator=(const Round &) = delete;
    Round(Round &&) = default;
    Round &operator=(Round &&) = default;
    ~Round() = default;

    int GetRoundIndex() const { return roundIndex_; }
    const std::shared_ptr<Move> GetWhiteMove() const { return whiteMove_; }
    const std::shared_ptr<Move> GetBlackMove() const { return blackMove_; }
    const std::string &GetComment() const { return roundComment_; }
    void Run(const std::shared_ptr<BoardGame> &boardGame) const;

  private:
    int roundIndex_;
    std::string roundComment_;
    std::shared_ptr<Move> whiteMove_;
    std::shared_ptr<Move> blackMove_;

  private:
    void ParseRoundText(const std::string &str);
};

using Rounds = std::vector<Round>;

} // namespace mlp_ha
