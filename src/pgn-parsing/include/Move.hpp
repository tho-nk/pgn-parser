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
    Move(/*const BoardGame &boardGame, */ const std::string &move, const std::string &comment);
    Move(const Move &) = delete;
    Move operator=(const Move &) = delete;
    ~Move() = default;

    std::pair<FromPosition, ToPosition> ExtractPosition();

  private:
    std::string move_;
    std::string comment_;
    // const BoardGame &boardGame_;
};
} // namespace mlp_ha
