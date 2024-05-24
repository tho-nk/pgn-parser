#pragma once

#include "move/include/Round.hpp"
#include "piece/include/Pieces.hpp"
#include "piece/include/Square.hpp"
#include <optional>
#include <variant>

namespace mlp_ha {

class BoardGame : public std::enable_shared_from_this<BoardGame> {
  public:
    BoardGame(const std::filesystem::path &filePath);
    BoardGame(const BoardGame &) = delete;
    BoardGame &operator=(const BoardGame &) = delete;
    BoardGame(BoardGame &&) = delete;
    BoardGame &operator=(BoardGame &&) = delete;
    ~BoardGame() = default;

    void Draw();
    void Run();

  private:
    std::shared_ptr<Square> square_;
};

} // namespace mlp_ha
