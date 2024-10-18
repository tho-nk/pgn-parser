#pragma once

#include "common/include/Types.hpp"
#include <cassert>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace mlp_ha {

/**
 * @brief Data structure that holds the information about a move.
 * This is helpful for moving forward and backward in the game.
 */
class MoveData {

  public:
    MoveData(const Color &color) : color_(color) {}
    virtual ~MoveData() = default;

    Color GetColor() const { return color_; }

    virtual void SetFromPosition(const FromPosition &) { assert(false); }
    virtual const FromPosition &GetFromPosition() const { assert(false); }

    virtual void SetToPosition(const ToPosition &) { assert(false); }
    virtual const ToPosition &GetToPosition() const { assert(false); }

    virtual void SetPieceType(const PieceType &) { assert(false); }
    virtual const PieceType &GetPieceType() const { assert(false); }

    virtual void SetPromotionType(const PieceType &) { assert(false); }
    virtual const PieceType &GetPromotionType() const { assert(false); }

    virtual void SetKingFromPosition(const Position &) { assert(false); }
    virtual const Position &GetKingFromPosition() const { assert(false); }

    virtual void SetKingToPosition(const Position &) { assert(false); }
    virtual const Position &GetKingToPosition() const { assert(false); }
    //
    virtual void SetRookFromPosition(const Position &) { assert(false); }
    virtual const Position &GetRookFromPosition() const { assert(false); }

    virtual void SetRookToPosition(const Position &) { assert(false); }
    virtual const Position &GetRookToPosition() const { assert(false); }

  private:
    Color color_;
};

class Move {
  public:
    Move(const MoveType &moveType, std::string &&moveText, std::string &&comment)
        : moveType_(moveType), moveText_(std::move(moveText)), comment_(std::move(comment)) {};
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
    std::unique_ptr<MoveData> moveData_ = nullptr;
};
} // namespace mlp_ha
