#include "move/include/PromotionMove.hpp"
#include "common/include/ParsingHelper.hpp"
#include "game/include/BoardGame.hpp"

namespace mlp_ha {

PromotionMove::PromotionMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {}

void PromotionMove::ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {
    std::cout << "PromotionMove::ProcessMove" << std::endl;
    auto str = moveText_;
    std::cout << moveText_ << std::endl;
    helper::removeUnwantedChars(str);

    // get pawn square,
    // get new piece type.
    std::string_view remain(str.data(), str.length() - 2);
    ToPosition toPosition{remain[1] - '1', remain[0] - 'a'};
    FromPosition fromPosition{toPosition.row - 1, toPosition.col};
    std::string pieceType(str.data() + str.length() - 1, 1);
    auto type = StringToPieceType(pieceType);
    boardGame->ProcessPromotionMove(type, this->color_, fromPosition, toPosition);
}

} // namespace mlp_ha
