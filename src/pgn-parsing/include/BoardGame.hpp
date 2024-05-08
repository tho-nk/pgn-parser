#pragma once
#include "Round.hpp"
#include <filesystem>

namespace mlp_ha {
class BoardGame {
  public:
    BoardGame(const std::filesystem::path &filePath);
    ~BoardGame() = default;

    void loadData();

  private:
    std::filesystem::path filePath_;
    Rounds rounds_;
};

} // namespace mlp_ha
