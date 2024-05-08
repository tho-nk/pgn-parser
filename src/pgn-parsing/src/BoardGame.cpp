#include "BoardGame.hpp"
#include "ParsingHelper.hpp"

namespace mlp_ha {
BoardGame::BoardGame(const std::filesystem::path &filePath) : filePath_(filePath) {}

void BoardGame::loadData() {
    auto roundQueue = ParseFile(filePath_);
    while (!roundQueue.empty()) {
        auto roundText = roundQueue.front();
        std::cout << "BoardGame::loadData roundText := " << roundText << std::endl;
        roundQueue.pop();
        rounds_.emplace_back(roundText);
        std::cout << std::endl;
    }
}

} // namespace mlp_ha
