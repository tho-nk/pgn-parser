#pragma once

#include "common/include/Types.hpp"
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>

namespace pgn {

template <typename ConcretePiece> class BasePiece {
  public:
    BasePiece(const BasePiece &) = delete;
    BasePiece &operator=(const BasePiece &) = delete;
    BasePiece(BasePiece &&) = default;
    BasePiece &operator=(BasePiece &&) = default;
    ~BasePiece() {}

    ConcretePiece &self() { return *static_cast<ConcretePiece *>(this); }
    ConcretePiece const &self() const { return *static_cast<ConcretePiece const *>(this); }

    std::string GetDraw() const {
        std::ostringstream ss;
        ss << color_ << type_;
        return ss.str();
    }

    void SetPosition(const Position &position) { this->position_ = position; }
    const Position &GetPosition() const { return position_; }

    void SetColor(const Color &color) { this->color_ = color; }
    const Color &GetColor() const { return color_; }

    void SetType(const PieceType &type) { type_ = type; }
    const PieceType &GetType() const { return type_; }

    bool IsValidBasicMove(const Position &toPosition,
                          const std::optional<Position> &validateKingCheck = std::nullopt) const {
        return self().IsValidBasicMove_(toPosition, validateKingCheck);
    }

    bool IsValidAttackMove(const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) const {
        return self().IsValidAttackMove_(toPosition, validateKingCheck);
    }

  protected:
    friend ConcretePiece;
    BasePiece() = default;

    PieceType type_{PieceType::Undefined};
    Color color_{Color::Undefined};
    Position position_{-1, -1};

    template <typename IsEmpty>
    bool ValidateMove_(int dRow, int dCol, const Position &toPosition, const std::optional<Position> &validateKingCheck,
                       Position p, const IsEmpty &isEmpty) const;
};

} // namespace pgn

#include "piece/include/BasePiece_impl.h"