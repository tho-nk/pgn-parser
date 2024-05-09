#pragma once

// #include "BoardGame.hpp"
#include "Pieces.hpp"
#include "Types.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <utility>
namespace mlp_ha {

// class BasePiece;
class BoardGame;

class Move {
  public:
    Move(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
        : moveType_(moveType), color_(color), moveText_(std::move(moveText)), comment_(std::move(comment)){};

    Move(const Move &) = delete;
    Move &operator=(const Move &) = delete;
    Move(Move &&) = delete;
    Move &operator=(Move &&) = delete;
    virtual ~Move() = default;

    std::pair<FromPosition, ToPosition> ExtractPosition();
    void SetMove(std::string move, std::string comment) {
        moveText_ = std::move(move);
        comment_ = std::move(comment);
    }

    const std::string &GetMoveText() const { return moveText_; }
    const Color &GetColor() const { return color_; }
    const std::string &GetComment() const { return comment_; }
    const MoveType &GetMoveType() const { return moveType_; }

    virtual void ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {};

  protected:
    MoveType moveType_;
    Color color_;
    std::string moveText_;
    std::string comment_;
    // const BoardGame &boardGame_;
};
} // namespace mlp_ha
