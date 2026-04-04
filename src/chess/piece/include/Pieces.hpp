#pragma once

#include "piece/include/Bishop.hpp"
#include "piece/include/EmptyPiece.hpp"
#include "piece/include/King.hpp"
#include "piece/include/Knight.hpp"
#include "piece/include/Pawn.hpp"
#include "piece/include/Queen.hpp"
#include "piece/include/Rook.hpp"
#include <array>
#include <cstddef>
#include <variant>

namespace pgn {

using Piece = std::variant<EmptyPiece, Bishop, King, Knight, Pawn, Queen, Rook>;
using Pieces = std::array<std::array<Piece, COLUMNS>, ROWS>;

struct PiecesReference {
    static constexpr std::size_t kMaxCandidates = 16;

    void push_back(const Piece &piece) {
        if (count < refs.size()) {
            refs[count++] = &piece;
        }
    }

    const Piece &at(const std::size_t idx) const { return *refs.at(idx); }

    auto begin() const { return refs.begin(); }
    auto end() const { return refs.begin() + static_cast<std::ptrdiff_t>(count); }

    std::size_t size() const { return count; }
    bool empty() const { return count == 0; }

  private:
    std::array<const Piece *, kMaxCandidates> refs{};
    std::size_t count = 0;
};

} // namespace pgn