#include "BoardGame.hpp"
#include "ParsingHelper.hpp"
#include <algorithm>

namespace mlp_ha {
BoardGame::BoardGame(const std::filesystem::path &filePath) : filePath_(filePath) {
    for (size_t r = 0; r < ROWS; ++r) {
        std::fill(std::begin(pieces_[r]), std::end(pieces_[r]), EmptyPiece{});
    }
    // Pawns
    for (auto c = 0; c < COLUMNS; ++c) {
        pieces_[1][c] = Pawn(Color::White, Position{1, c});
        pieces_[6][c] = Pawn(Color::Black, Position{6, c});
    }
    // Rooks
    pieces_[0][0] = Rook(Color::White, Position{0, 0});
    pieces_[0][7] = Rook(Color::White, Position{0, 7});
    pieces_[7][0] = Rook(Color::Black, Position{7, 0});
    pieces_[7][7] = Rook(Color::Black, Position{7, 7});

    // Knights
    pieces_[0][1] = Knight(Color::White, Position{0, 1});
    pieces_[0][6] = Knight(Color::White, Position{0, 6});
    pieces_[7][1] = Knight(Color::Black, Position{7, 1});
    pieces_[7][6] = Knight(Color::Black, Position{7, 6});

    // Bishops
    pieces_[0][2] = Bishop(Color::White, Position{0, 2});
    pieces_[0][5] = Bishop(Color::White, Position{0, 5});
    pieces_[7][2] = Bishop(Color::Black, Position{7, 2});
    pieces_[7][5] = Bishop(Color::Black, Position{7, 5});

    // Queens
    pieces_[0][3] = Queen(Color::Black, Position{7, 3});
    pieces_[7][3] = Queen(Color::Black, Position{7, 3});

    // Kings
    pieces_[0][4] = King(Color::White, Position{0, 4});
    pieces_[7][4] = King(Color::Black, Position{0, 4});
}

Piece BoardGame::GetPiece(const Move &move) {
    FromPosition from;
    ToPosition to;
    Piece piece = EmptyPiece{};
    const auto moveText = move.GetMove();
}

void BoardGame::Run() {
    for (const auto &round : rounds_) {
        auto roundIndex = round.GetRoundIndex();
        const auto &whiteMove = round.GetWhiteMove();
        // const auto whitePice = GetPiece(whiteMove);
        const auto &blackMove = round.GetBlackMove();
        std::cout << "round:=" << roundIndex << ", roundComment:=" << round.GetComment()
                  << ", whiteMove:=" << whiteMove.GetMove() << ", whiteComment:=" << whiteMove.GetComment()
                  << ", blackMove:=" << blackMove.GetMove() << ", blackComment:=" << blackMove.GetComment()
                  << std::endl;
    }
}

void BoardGame::Draw() {
    for (int r = ROWS - 1; r >= 0; --r) {
        for (auto c = 0; c < COLUMNS - 1; ++c) {
            std::visit(
                [](auto &&piece) {
                    std::cout << piece.GetDraw();
                    std::cout << "|";
                },
                pieces_[r][c]);
        }
        std::visit([](auto &&piece) { std::cout << piece.GetDraw(); }, pieces_[r][COLUMNS - 1]);
        std::cout << std::endl;
    }
}

void BoardGame::LoadData() {
    auto roundQueue = helper::ParseFile(filePath_);
    while (!roundQueue.empty()) {
        auto roundText = roundQueue.front();
        roundQueue.pop();
        rounds_.emplace_back(roundText);
    }
}

} // namespace mlp_ha
