#include "game/include/BoardGame.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        // std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }
    std::filesystem::path filePath = argv[1];
    auto game = std::make_shared<mlp_ha::BoardGame>(filePath);
    game->LoadData();
    game->Run();
    game->Draw();
    return 0;
}
