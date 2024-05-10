#include "AttackMove.hpp"
#include "BoardGame.hpp"
#include "ParsingHelper.hpp"
#include <ranges>

namespace mlp_ha {

AttackMove::AttackMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {}

void AttackMove::ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {
    std::cout << "AttackMove::ProcessMove" << std::endl;
    auto str = moveText_;
    std::cout << moveText_ << std::endl;
    helper::removeUnwantedChars(str);
    std::string_view remain(str.data(), str.length() - 3); // exclude 'x'
    std::string_view dest(str.data() + str.length() - 2, 2);
    ToPosition toPosition{dest[1] - '1', dest[0] - 'a'};
    FromPosition fromPosition{-1, -1};
    std::string pieceType = "P";
    if (std::isupper(remain[0])) {
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
    } else {
        fromPosition.col = remain[0] - 'a';
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
    boardGame->ProcessAttackMove(subPieces, toPosition, fromPosition);
    boardGame->AttackPiece(fromPosition, toPosition);
}

} // namespace mlp_ha
