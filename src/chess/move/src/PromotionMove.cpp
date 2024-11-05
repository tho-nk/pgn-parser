#include "move/include/PromotionMove.hpp"
#include "common/include/ParsingHelper.hpp"
#include "common/include/PgnException.hpp"
#include "piece/include/Square.hpp"

namespace pgn {

PromotionMove::PromotionMove(const MoveType &moveType, const Color &color, std::string &&moveText,
                             std::string &&comment)
    : Move(moveType, color, std::move(moveText), std::move(comment)) {
    ComputeMoveData();
}

void PromotionMove::ComputeMoveData() {
    // std::clog << "[THO][I] PromotionMove::ProcessMove" << std::endl;
    auto str = moveText_;
    // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
    helper::removeUnwantedChars(str);

    std::string_view remain(str.data(), str.length() - 2);
    moveData_.toPosition = ToPosition{remain[1] - '1', remain[0] - 'a'};
    moveData_.fromPosition =
        FromPosition{moveData_.color == Color::White ? moveData_.toPosition.row - 1 : moveData_.toPosition.row + 1,
                     moveData_.toPosition.col};
    std::string promotionType(str.data() + str.length() - 1, 1);
    moveData_.promotionType = StringToPieceType(promotionType);
}

void PromotionMove::ProcessMove() {
    pgn::Square::GetInstance().ProcessPromotionMove(moveData_.promotionType, moveData_.color, moveData_.fromPosition,
                                                    moveData_.toPosition);
}

} // namespace pgn
