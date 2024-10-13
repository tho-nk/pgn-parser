#include "move/include/PromotionMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "piece/include/Square.hpp"

namespace mlp_ha {

PromotionMove::PromotionMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {
    ComputeMoveData();
}

void PromotionMove::ComputeMoveData() {
    // std::clog << "[THO][I] PromotionMove::ProcessMove" << std::endl;
    auto str = moveText_;
    // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
    helper::removeUnwantedChars(str);

    std::string_view remain(str.data(), str.length() - 2);
    moveData_.toPosition = Position{remain[1] - '1', remain[0] - 'a'};
    moveData_.fromPosition =
        Position{moveData_.color == Color::White ? moveData_.toPosition.row - 1 : moveData_.toPosition.row + 1,
                 moveData_.toPosition.col};
    std::string promotionType(str.data() + str.length() - 1, 1);
    moveData_.promotionType = StringToPieceType(promotionType);
}

void PromotionMove::ProcessMove(Square *square) {
    square->ProcessPromotionMove(moveData_.promotionType, moveData_.color, moveData_.fromPosition,
                                 moveData_.toPosition);
}

} // namespace mlp_ha
