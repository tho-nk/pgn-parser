#pragma once

#include "common/include/Types.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace pgn {

class MoveBase {
  public:
    MoveBase(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment)
        : moveType_(moveType), moveData_(color), moveText_(std::move(moveText)), comment_(std::move(comment)) {};
    MoveBase() = default;
    MoveBase(const MoveBase &) = delete;
    MoveBase &operator=(const MoveBase &) = delete;
    MoveBase(MoveBase &&) = default;
    MoveBase &operator=(MoveBase &&) = default;
    ~MoveBase() = default;

    void SetMove(std::string move, std::string comment) {
        moveText_ = std::move(move);
        comment_ = std::move(comment);
    }

    const std::string &GetMoveText() const { return moveText_; }
    const std::string &GetComment() const { return comment_; }
    const MoveType &GetMoveType() const { return moveType_; }

    void Process(this auto &&self) { self.ProcessMove(); };

    void ProcessMove() const {}

  protected:
    MoveType moveType_{MoveType::Undefined};
    std::string moveText_;
    std::string comment_;
    mutable MoveData moveData_{Color::Undefined};
};
} // namespace pgn
