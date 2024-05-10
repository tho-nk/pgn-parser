#include "BasicMove.hpp"
#include "BoardGame.hpp"
#include "ParsingHelper.hpp"
#include <ranges>

namespace mlp_ha {

BasicMove::BasicMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {}

void BasicMove::ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {
    std::cout << "BasicMove::ProcessMove" << std::endl;
    auto str = moveText_;
    std::cout << moveText_ << std::endl;
    helper::removeUnwantedChars(str);
    std::string_view remain(str.data(), str.length() - 2);
    std::string_view dest(str.data() + str.length() - 2, 2);
    ToPosition toPosition{dest[1] - '1', dest[0] - 'a'};
    FromPosition fromPosition{-1, -1};
    std::string pieceType = "P";
    if (!remain.empty()) {
        pieceType = remain.at(0);
        int index = 1;
        while (index < remain.length()) {
            if (std::isdigit(remain[index])) {
                fromPosition.row = remain[index] - '1';
            } else {
                fromPosition.col = remain[index] - 'a';
            }
            index++;
        }
    }
    auto type = StringToPieceType(pieceType);

    PiecesReference subPieces;
    if (fromPosition.IsValid()) {
        subPieces.push_back(std::ref(boardGame->GetPieces()[fromPosition.row][fromPosition.col]));
    } else {
        auto begin = boardGame->GetPieces().front().begin();
        auto end = boardGame->GetPieces().back().end();
        auto arange =
            std::ranges::subrange(boardGame->GetPieces().front().begin(), boardGame->GetPieces().back().end());
        for (const auto &var : arange) {
            std::visit(
                [&](const auto &value) {
                    if (value.GetType() == type && value.GetColor() == this->color_) {
                        subPieces.push_back(
                            std::ref(boardGame->GetPieces()[value.GetPosition().row][value.GetPosition().col]));
                    }
                },
                var);
        }
    }

    // Piece aPiece(CreatePiece(pieceType, this->color_, toPosition));
    // std::visit([&](const auto &piece) { boardGame->ProcessMove(piece, fromPosition); }, aPiece);

    boardGame->ProcessBasicMove(subPieces, toPosition, fromPosition);
    boardGame->MovePiece(fromPosition, toPosition);
    // for (auto &it : subPieces) {
    //     std::visit([&](auto &&piece) { std::cout << "THOTHO:=" << piece.GetDraw(); }, it.get());
    //     std::visit([&](auto &&piece) { fromPosition = boardGame->ComputeFromPosition(piece, toPosition); },
    //     it.get());
    // }
}

} // namespace mlp_ha
