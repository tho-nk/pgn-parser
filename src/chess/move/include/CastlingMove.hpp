#pragma once

#include "move/include/Move.hpp"

namespace mlp_ha {

class CastlingMoveData : public MoveData {
  public:
    CastlingMoveData(const Color &color)
        : MoveData(color), toPositionKing_(), fromKing_(), toPositionRook_(), fromRook_() {}
    CastlingMoveData(const CastlingMoveData &) = delete;
    CastlingMoveData &operator=(const CastlingMoveData &) = delete;
    CastlingMoveData(CastlingMoveData &&) = delete;
    CastlingMoveData &operator=(CastlingMoveData &&) = delete;
    ~CastlingMoveData() = default;

    virtual void SetKingFromPosition(const Position &p) { fromKing_ = p; }
    virtual const Position &GetKingFromPosition() const { return fromKing_; }

    virtual void SetKingToPosition(const Position &p) { toPositionKing_ = p; }
    virtual const Position &GetKingToPosition() const { return toPositionKing_; }
    //
    virtual void SetRookFromPosition(const Position &p) { fromRook_ = p; }
    virtual const Position &GetRookFromPosition() const { return fromRook_; }

    virtual void SetRookToPosition(const Position &p) { toPositionRook_ = p; }
    virtual const Position &GetRookToPosition() const { return toPositionRook_; }

  private:
    FromPosition fromKing_;
    ToPosition toPositionKing_;

    FromPosition fromRook_;
    ToPosition toPositionRook_;
};

class CastlingMove : public Move {
  public:
    CastlingMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment);
    CastlingMove(const CastlingMove &) = delete;
    CastlingMove &operator=(const CastlingMove &) = delete;
    CastlingMove(CastlingMove &&) = delete;
    CastlingMove &operator=(CastlingMove &&) = delete;
    ~CastlingMove() = default;

    virtual void ProcessMove() final;

  private:
    void ComputeMoveData();
};
} // namespace mlp_ha
