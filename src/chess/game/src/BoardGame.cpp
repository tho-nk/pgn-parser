#include "game/include/BoardGame.hpp"
#include "common/include/ParsingHelper.hpp"
#include <algorithm>

namespace mlp_ha {
BoardGame::BoardGame(const std::filesystem::path &filePath) : square_(std::make_shared<Square>(filePath)) {
    square_->InitSquare();
    square_->LoadData();
}

void BoardGame::Run() { square_->Run(); }

void BoardGame::Draw() { std::cout << square_->GetCurrentState() << std::endl; }

} // namespace mlp_ha
