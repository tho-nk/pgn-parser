#include "pgn-parsing/include/BoardGame.hpp"
#include "pgn-parsing/include/ParsingHelper.hpp"
#include <iostream>

void printQueue(const std::queue<std::string> &q) {
    std::queue<std::string> temp = q; // Make a copy of the original queue

    // Print elements while the temporary queue is not empty
    while (!temp.empty()) {
        std::cout << temp.front() << std::endl;
        temp.pop();
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }
    std::filesystem::path filePath = argv[1];
    mlp_ha::BoardGame game{filePath};
    game.LoadData();
    game.Draw();
    game.Run();
    // auto q = mlp_ha::ParseFile(filePath);
    // printQueue(q);
    return 0;
}
