#include "move/include/AttackPromotionMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "piece/include/Square.hpp"

namespace mlp_ha {

AttackPromotionMove::AttackPromotionMove(const MoveType &moveType, const Color &color, std::string &&moveText,
                                         std::string &&comment)
    : Move(moveType, std::move(moveText), std::move(comment)) {
    moveData_ = std::make_unique<AttackPromotionMoveData>(color);
    ComputeMoveData();
}

void AttackPromotionMove::ComputeMoveData() {
    // std::clog << "[THO][I] AttackPromotionMove::ProcessMove" << std::endl;
    auto str = moveText_;
    // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
    helper::removeUnwantedChars(str);

    auto foundA = str.find("x");
    auto foundP = str.find("=");
    // foundA + 3 == foundP
    std::string from = str.substr(0, foundA); // exclude 'x'
    std::string dest = str.substr(foundA + 1, foundP - foundA - 1);
    std::string promotionType = str.substr(foundP + 1);

    moveData_->SetToPosition(Position{dest[1] - '1', dest[0] - 'a'});
    std::string pieceType = "P"; // only for Pawn
    FromPosition fromPosition{-1, -1};

    if (std::isupper(from[0])) {
        pieceType = from.at(0); // should be Pawn
        int index = 1;
        while (index < from.length()) {
            if (std::isdigit(from[index])) {
                fromPosition.row = from[index] - '1';
            } else {
                fromPosition.col = from[index] - 'a';
            }
            index++;
        }
    } else {
        fromPosition.col = from[0] - 'a';
    }

    moveData_->SetFromPosition(fromPosition);
    moveData_->SetPieceType(StringToPieceType(pieceType));
    moveData_->SetPromotionType(StringToPieceType(promotionType));
}
// exf8=R+
void AttackPromotionMove::ProcessMove() {
    AttackMove::ProcessMove();
    PromotionMove::ProcessMove();
}

} // namespace mlp_ha
