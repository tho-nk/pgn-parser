#include "game/include/BoardGame.hpp"
#include <csignal>
#include <iostream>

void signalHandler(int signal) {
    std::clog << "[THO][I] Stop Parsing PGN with signal:= " << signal << std::endl;
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "[THO][E] this program accepts only one parameter" << std::endl;
        return 1;
    }

    signal(SIGINT, signalHandler);
    std::filesystem::path filePath = argv[1];
    try {
        auto game = std::make_shared<mlp_ha::BoardGame>(filePath);
        // game->LoadData();
        game->Run();
        game->Draw();
    } catch (const std::exception &e) {
        std::cerr << "[THO][E] Error while parsing PGN : " << e.what() << std::endl;
    }
    return 0;
}
