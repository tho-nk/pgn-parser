#include "PromotionMove.hpp"
#include "BoardGame.hpp"
#include "ParsingHelper.hpp"

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
    std::string_view dest(str.data() + str.length() - 2, 2);
    ToPosition toPosition{dest[1] - '1', dest[0] - 'a'};
    FromPosition fromPosition{-1, -1};
}

} // namespace mlp_ha
