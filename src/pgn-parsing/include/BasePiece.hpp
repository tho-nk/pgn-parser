#pragma once

#include "Types.hpp"
#include <sstream>

namespace mlp_ha {

template <typename ConcretePiece> class BasePiece {
  public:
    BasePiece() : type_(PieceType::Undefined), color_(Color::Undefined) {}
    BasePiece(const BasePiece &) = delete;
    BasePiece &operator=(const BasePiece &) = delete;
    BasePiece(BasePiece &&) = default;
    BasePiece &operator=(BasePiece &&) = default;
    ~BasePiece() = default;

    ConcretePiece &GetPiece() { return *static_cast<ConcretePiece *>(this); }
    ConcretePiece const &GetPiece() const { return *static_cast<ConcretePiece const *>(this); }

    std::string GetDraw() const {
        std::ostringstream ss;
        ss << color_ << type_;
        return ss.str();
    }

    void SetPosition(const Position &position) { this->position_ = position; }

    const Position &GetPosition() const { return position_; }

    const Color &GetColor() const { return color_; }

    const PieceType &GetType() const { return type_; }
    // Positions ComputeFromPositions(const std::string &move) {
    //     std::cout << "BasePiece::ComputeFromPositions" << std::endl;
    //     return {}; // GetPiece().ComputeFromPositions(move);
    // }

  protected:
    PieceType type_;
    Color color_;
    Position position_;
};

} // namespace mlp_ha
