#pragma once

#include "Types.hpp"
#include <memory>
#include <sstream>

namespace mlp_ha {
// class BoardGame;

template <typename ConcretePiece> class BasePiece {
  public:
    BasePiece(const BasePiece &) = delete;
    BasePiece &operator=(const BasePiece &) = delete;
    BasePiece(BasePiece &&) = default;
    BasePiece &operator=(BasePiece &&) = default;
    ~BasePiece() = default;

    ConcretePiece &self() { return *static_cast<ConcretePiece *>(this); }
    ConcretePiece const &self() const { return *static_cast<ConcretePiece const *>(this); }

    std::string GetDraw() const {
        std::ostringstream ss;
        ss << color_ << type_;
        return ss.str();
    }

    void SetPosition(const Position &position) { this->position_ = position; }

    const Position &GetPosition() const { return position_; }

    const Color &GetColor() const { return color_; }

    const PieceType &GetType() const { return type_; }

    bool IsValidBasicMove(const Position &position) const { return self().IsValidBasicMove_(position); }

  protected:
    friend ConcretePiece;
    BasePiece() : type_(PieceType::Undefined), color_(Color::Undefined) {}

    PieceType type_;
    Color color_;
    Position position_;
};

} // namespace mlp_ha
