#include "BoardGame.hpp"
#include "ParsingHelper.hpp"
#include <algorithm>

namespace mlp_ha {
BoardGame::BoardGame(const std::filesystem::path &filePath) : filePath_(filePath) {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLUMNS; ++c) {
            pieces_[r][c].emplace<EmptyPiece>(Color::Undefined, Position{r, c});
        }
    }
    // Pawns
    for (auto c = 0; c < COLUMNS; ++c) {
        pieces_[1][c].emplace<Pawn>(Color::White, Position{1, c});
        pieces_[6][c].emplace<Pawn>(Color::Black, Position{6, c});
    }
    // Rooks
    pieces_[0][0].emplace<Rook>(Color::White, Position{0, 0});
    pieces_[0][7].emplace<Rook>(Color::White, Position{0, 7});
    pieces_[7][0].emplace<Rook>(Color::Black, Position{7, 0});
    pieces_[7][7].emplace<Rook>(Color::Black, Position{7, 7});

    // Knights
    pieces_[0][1].emplace<Knight>(Color::White, Position{0, 1});
    pieces_[0][6].emplace<Knight>(Color::White, Position{0, 6});
    pieces_[7][1].emplace<Knight>(Color::Black, Position{7, 1});
    pieces_[7][6].emplace<Knight>(Color::Black, Position{7, 6});

    // Bishops
    pieces_[0][2].emplace<Bishop>(Color::White, Position{0, 2});
    pieces_[0][5].emplace<Bishop>(Color::White, Position{0, 5});
    pieces_[7][2].emplace<Bishop>(Color::Black, Position{7, 2});
    pieces_[7][5].emplace<Bishop>(Color::Black, Position{7, 5});

    // Queens
    pieces_[0][3].emplace<Queen>(Color::White, Position{7, 3});
    pieces_[7][3].emplace<Queen>(Color::Black, Position{7, 3});

    // Kings
    pieces_[0][4].emplace<King>(Color::White, Position{0, 4});
    pieces_[7][4].emplace<King>(Color::Black, Position{0, 4});

    // Draw();
    // std::cout << "NEXT" << std::endl;
    // pieces_[0][4].swap(pieces_[7][4]);
    // Draw();
    // std::cout << "NEXT" << std::endl;
    // pieces_[0][4].swap(pieces_[7][4]);
    // Draw();
    // std::cout << "NEXT" << std::endl;
    // pieces_[0][4].swap(pieces_[5][3]);
    // Draw();
    // std::cout << "NEXT" << std::endl;
    // pieces_[1][4].emplace<EmptyPiece>();
    // Draw();
}

Piece BoardGame::GetPiece(const Move &move) {
    // FromPosition from;
    // ToPosition to;
    // Piece piece{EmptyPiece{}};
    // const auto moveText = move.GetMove();
}

void BoardGame::Run() {
    for (const auto &round : rounds_) {
        auto roundIndex = round.GetRoundIndex();
        const auto &whiteMove = round.GetWhiteMove();
        // const auto whitePice = GetPiece(whiteMove);
        const auto &blackMove = round.GetBlackMove();
        std::cout << "round:=" << roundIndex << std::endl;

        /*std::cout << "round:=" << roundIndex << ", roundComment:=" << round.GetComment()
        << ", whiteMoveType:=" << whiteMove->GetMoveType() << ", whiteMove:=" << whiteMove->GetMoveText()
        << ", whiteComment:=" << whiteMove->GetComment() << ", blackMoveType:=" << blackMove->GetMoveType()
        << ", blackMove:=" << blackMove->GetMoveText() << ", blackComment:=" << blackMove->GetComment() << std::endl;
        */
        whiteMove->ProcessMove(shared_from_this());
        blackMove->ProcessMove(shared_from_this());

        std::cout << std::endl;
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

void BoardGame::ProcessMove(const King &king, const FromPosition &fromPosition) {
    std::cout << "Begin ProcessMove King" << std::endl;
    std::cout << "King: fromPosition.row:=" << fromPosition.row << ", fromPosition.col:=" << fromPosition.col
              << ", King.row:=" << king.GetPostion().row << ", King.col:=" << king.GetPostion().col << std::endl;
    std::cout << "End ProcessMove king\n" << std::endl;
}
void BoardGame::ProcessMove(const Queen &queen, const FromPosition &fromPosition) {
    std::cout << "Begin ProcessMove queen" << std::endl;
    std::cout << "queen: fromPosition.row:=" << fromPosition.row << ", fromPosition.col:=" << fromPosition.col
              << ", queen.row:=" << queen.GetPostion().row << ", queen.col:=" << queen.GetPostion().col << std::endl;
    std::cout << "End ProcessMove queen\n" << std::endl;
}
void BoardGame::ProcessMove(const Rook &rook, const FromPosition &fromPosition) {
    std::cout << "Begin ProcessMove rook" << std::endl;
    std::cout << "rook: fromPosition.row:=" << fromPosition.row << ", fromPosition.col:=" << fromPosition.col
              << ", rook.row:=" << rook.GetPostion().row << ", rook.col:=" << rook.GetPostion().col << std::endl;
    std::cout << "End ProcessMove rook\n" << std::endl;
}
void BoardGame::ProcessMove(const Bishop &bishop, const FromPosition &fromPosition) {
    std::cout << "Begin ProcessMove bishop" << std::endl;
    std::cout << "bishop: fromPosition.row:=" << fromPosition.row << ", fromPosition.col:=" << fromPosition.col
              << ", bishop.row:=" << bishop.GetPostion().row << ", bishop.col:=" << bishop.GetPostion().col
              << std::endl;
    std::cout << "End ProcessMove bishop\n" << std::endl;
}
void BoardGame::ProcessMove(const Knight &knight, const FromPosition &fromPosition) {
    std::cout << "Begin ProcessMove knight" << std::endl;
    std::cout << "knight: fromPosition.row:=" << fromPosition.row << ", fromPosition.col:=" << fromPosition.col
              << ", knight.row:=" << knight.GetPostion().row << ", knight.col:=" << knight.GetPostion().col
              << std::endl;
    std::cout << "End ProcessMove Pawn\n" << std::endl;
}
void BoardGame::ProcessMove(const Pawn &pawn, const FromPosition &fromPosition) {
    std::cout << "Begin ProcessMove Pawn" << std::endl;
    std::cout << "Pawn: fromPosition.row:=" << fromPosition.row << ", fromPosition.col:=" << fromPosition.col
              << ", pawn.row:=" << pawn.GetPostion().row << ", pawn.col:=" << pawn.GetPostion().col << std::endl;
    std::cout << "End ProcessMove Pawn\n" << std::endl;
}
void BoardGame::ProcessMove(const EmptyPiece &empty, const FromPosition &fromPosition) {}

} // namespace mlp_ha
