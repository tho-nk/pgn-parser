#include "game/include/BoardGame.hpp"
#include "common/include/ParsingHelper.hpp"
#include <algorithm>

namespace mlp_ha {
BoardGame::BoardGame(const std::filesystem::path &filePath) : filePath_(filePath), square_(std::make_shared<Square>()) {
    square_->InitSquare();
}

void BoardGame::Run() {
    for (const auto &round : rounds_) {
        round.Run(shared_from_this());
    }
    // Draw();
}

void BoardGame::Draw() { square_->Draw(); }

void BoardGame::LoadData() {
    auto parsingHelper = helper::ParseFile(filePath_);
    while (!parsingHelper.roundQueue.empty()) {
        auto roundText = parsingHelper.roundQueue.front();
        parsingHelper.roundQueue.pop();
        rounds_.emplace_back(roundText);
    }

    auto found = parsingHelper.lastRun.find("1-0");
    if (found == std::string::npos) {
        found = parsingHelper.lastRun.find("0-1");
    }
    if (found == std::string::npos) {
        found = parsingHelper.lastRun.find("1/2-1/2");
    }
    if (found == std::string::npos) {
        // std::cerr << "Error result" << std::endl;
    }
    auto roundText = parsingHelper.lastRun.substr(0, found);
    // std::cout << "roundText:=" << roundText << std::endl;
    rounds_.emplace_back(roundText);
}

void BoardGame::ProcessBasicMove(const PiecesReference &subPieces, const ToPosition &toPosition,
                                 FromPosition &fromPosition) {
    square_->ProcessBasicMove(subPieces, toPosition, fromPosition);
}

void BoardGame::MovePiece(const FromPosition &fromPosition, const ToPosition toPosition) {
    square_->MovePiece(fromPosition, toPosition);
}

void BoardGame::AttackPiece(const FromPosition &fromPosition, const ToPosition toPosition) {
    square_->AttackPiece(fromPosition, toPosition);
}

void BoardGame::ProcessAttackMove(const PiecesReference &subPieces, const ToPosition &toPosition,
                                  FromPosition &fromPosition) {
    square_->ProcessAttackMove(subPieces, toPosition, fromPosition);
}

void BoardGame::ProcessPromotionMove(const PieceType &pieceType, const Color &color, const FromPosition &fromPosition,
                                     const ToPosition &toPosition) {

    square_->ProcessPromotionMove(pieceType, color, fromPosition, toPosition);
}

PiecesReference BoardGame::GetPieceOfTypeAndColor(const PieceType &pieceType, const Color &color,
                                                  const FromPosition &fromPosition) {
    return square_->GetPieceOfTypeAndColor(pieceType, color, fromPosition);
}
} // namespace mlp_ha
