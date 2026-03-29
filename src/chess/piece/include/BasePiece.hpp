#pragma once

#include "common/include/Types.hpp"
#include <concepts>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>

namespace pgn {

struct PieceState {
    PieceType type{PieceType::Undefined};
    Color color{Color::Undefined};
    Position position{-1, -1};
};

template <typename T>
concept BasicMoveValidator =
    requires(const T &piece, const Position &toPosition, const std::optional<Position> &validateKingCheck) {
        { piece.IsValidBasicMove_(toPosition, validateKingCheck) } -> std::convertible_to<bool>;
    };

template <typename T>
concept AttackMoveValidator =
    requires(const T &piece, const Position &toPosition, const std::optional<Position> &validateKingCheck) {
        { piece.IsValidAttackMove_(toPosition, validateKingCheck) } -> std::convertible_to<bool>;
    };

template <typename Derived> class BasePiece {
  public:
    BasePiece(const BasePiece &) = delete;
    BasePiece &operator=(const BasePiece &) = delete;
    BasePiece(BasePiece &&) = default;
    BasePiece &operator=(BasePiece &&) = default;
    ~BasePiece() = default;

    std::string GetDraw() const {
        std::ostringstream ss;
        ss << state_.color << state_.type;
        return ss.str();
    }

    void SetPosition(const Position &position) { this->state_.position = position; }
    const Position &GetPosition() const { return state_.position; }

    void SetColor(const Color &color) { this->state_.color = color; }
    const Color &GetColor() const { return state_.color; }

    void SetType(const PieceType &type) { state_.type = type; }
    const PieceType &GetType() const { return state_.type; }

    template <typename T = Derived>
    requires BasicMoveValidator<T>
    bool IsValidBasicMove(this const Derived &self, const Position &toPosition,
                          const std::optional<Position> &validateKingCheck = std::nullopt) {
        return self.IsValidBasicMove_(toPosition, validateKingCheck);
    }

    template <typename T = Derived>
    requires AttackMoveValidator<T>
    bool IsValidAttackMove(this const Derived &self, const Position &toPosition,
                           const std::optional<Position> &validateKingCheck = std::nullopt) {
        return self.IsValidAttackMove_(toPosition, validateKingCheck);
    }

  protected:
    BasePiece() = default;

    PieceState state_{};

    template <typename IsEmpty>
    bool ValidateMove_(int dRow, int dCol, const Position &toPosition, const std::optional<Position> &validateKingCheck,
                       Position p, const IsEmpty &isEmpty) const;
};

} // namespace pgn

#include "piece/include/BasePiece_impl.h"