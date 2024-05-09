#pragma once

#include "Types.hpp"
#include <iostream>

namespace mlp_ha {

template <typename ConcretePiece> class BasePiece {
  public:
    BasePiece() : type_(" "), color_(Color::Undefined) {}
    ~BasePiece() = default;

    ConcretePiece &GetPiece() { return *static_cast<ConcretePiece *>(this); }
    ConcretePiece const &GetPiece() const { return *static_cast<ConcretePiece const *>(this); }

    std::string GetDraw() {
        std::string result = "";
        if (color_ == Color::White) {
            result += "w";
        } else if (color_ == Color::Black) {
            result += "b";
        } else {
            result += " ";
        }
        result += type_;
        return result;
    }

    void SetPosition(const Position &position) { this->position_ = position; }

    Position GetPostion() const { return position_; }

    Color GetColor() const { return color_; }

    Positions ComputeFromPositions(const std::string &move) { return GetPiece().ComputeFromPositions(move); }

  protected:
    PieceType type_;
    Color color_;
    Position position_;
};

} // namespace mlp_ha
