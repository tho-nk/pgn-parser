#pragma once

#include "Pieces.hpp"
#include "Round.hpp"
#include <filesystem>
#include <variant>

namespace mlp_ha {

class BoardGame {
  public:
    BoardGame(const std::filesystem::path &filePath);
    ~BoardGame() = default;

    void LoadData();
    void Draw();
    void Run();

  private:
    std::filesystem::path filePath_;
    Rounds rounds_;
    Pieces pieces_;
};

} // namespace mlp_ha
