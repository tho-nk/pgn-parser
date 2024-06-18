#pragma once

#include "common/include/Types.hpp"
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>

namespace mlp_ha {
class Square;

template <typename ConcretePiece> class BasePiece {
  public:
    BasePiece(const BasePiece &) = delete;
    BasePiece &operator=(const BasePiece &) = delete;
    BasePiece(BasePiece &&) = default;
    BasePiece &operator=(BasePiece &&) = default;
    ~BasePiece() { square_.reset(); }

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

    void SetSquare(const std::shared_ptr<Square> &square) { this->square_ = square; }
    const std::shared_ptr<Square> &GetSquare() const { return square_; }

    void SeType(const PieceType &type) { type_ = type; }
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
    BasePiece(const std::shared_ptr<Square> &square = nullptr)
        : square_(square), type_(PieceType::Undefined), color_(Color::Undefined) {}

    PieceType type_;
    Color color_;
    Position position_;
    std::shared_ptr<Square> square_;
};

} // namespace mlp_ha
