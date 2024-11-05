#pragma once

#include "common/include/Types.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace pgn {

class Move {
  public:
    Move(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment)
        : moveType_(moveType), moveData_(color), moveText_(std::move(moveText)), comment_(std::move(comment)) {};
    Move() = default;
    Move(const Move &) = delete;
    Move &operator=(const Move &) = delete;
    Move(Move &&) = delete;
    Move &operator=(Move &&) = delete;
    virtual ~Move() = default;

    void SetMove(std::string move, std::string comment) {
        moveText_ = std::move(move);
        comment_ = std::move(comment);
    }

    const std::string &GetMoveText() const { return moveText_; }
    const std::string &GetComment() const { return comment_; }
    const MoveType &GetMoveType() const { return moveType_; }

    virtual void ProcessMove() {};

  protected:
    MoveType moveType_{MoveType::Undefined};
    std::string moveText_;
    std::string comment_;
    MoveData moveData_{Color::Undefined};
};
} // namespace pgn
