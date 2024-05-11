#include "move/include/BasicMove.hpp"
#include "common/include/ParsingHelper.hpp"
#include "game/include/BoardGame.hpp"
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

    const auto subPieces = boardGame->GetPieceOfTypeAndColor(type, this->color_, fromPosition);
    boardGame->ProcessBasicMove(subPieces, toPosition, fromPosition);
    boardGame->MovePiece(fromPosition, toPosition);
}

} // namespace mlp_ha
