#pragma once

// #include "BoardGame.hpp"
#include "Types.hpp"
#include <string>
#include <string_view>
#include <utility>

namespace mlp_ha {

// class BasePiece;
// class BoardGame;

class Move {
  public:
    Move() = default;
    Move(const Move &) = delete;
    Move operator=(const Move &) = delete;
    Move(Move &&) = delete;
    Move operator=(Move &&) = delete;
    ~Move() = default;

    std::pair<FromPosition, ToPosition> ExtractPosition();
    void SetMove(std::string move, std::string comment) {
        move_ = std::move(move);
        comment_ = std::move(comment);
    }

    const std::string &GetMove() const { return move_; }
    const std::string &GetComment() const { return comment_; }

  private:
    std::string move_;
    std::string comment_;
    // const BoardGame &boardGame_;
};
} // namespace mlp_ha
