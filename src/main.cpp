#include "common/include/MlpException.hpp"
#include "piece/include/Square.hpp"
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
        auto square = mlp_ha::Square(filePath);
        square.Init();
        square.LoadPGN();
        square.Run();
        std::cout << square.GetCurrentState() << std::endl;
    } catch (const mlp_ha::MlpException &e) {
        std::cerr << "[THO][E] MlpException " << e.what() << " for file: " << filePath << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[THO][E] Error while parsing PGN : " << e.what() << " for file: " << filePath << std::endl;
    } catch (...) {
        std::cerr << "[THO][E] unkown exception for file: " << filePath << std::endl;
    }
    return 0;
}
