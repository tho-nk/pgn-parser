#include "move/include/AttackPromotionMove.hpp"
#include "common/include/ParsingHelper.hpp"
#include "common/include/PgnException.hpp"
#include "piece/include/Square.hpp"

namespace pgn {

AttackPromotionMove::AttackPromotionMove(const MoveType &moveType, const Color &color, std::string &&moveText,
                                         std::string &&comment)
    : Move(moveType, color, std::move(moveText), std::move(comment)) {
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

    moveData_.toPosition = Position{dest[1] - '1', dest[0] - 'a'};
    std::string pieceType = "P"; // only for Pawn
    if (std::isupper(from[0])) {
        pieceType = from.at(0); // should be Pawn
        int index = 1;
        while (index < from.length()) {
            if (std::isdigit(from[index])) {
                moveData_.fromPosition.row = from[index] - '1';
            } else {
                moveData_.fromPosition.col = from[index] - 'a';
            }
            index++;
        }
    } else {
        moveData_.fromPosition.col = from[0] - 'a';
    }

    moveData_.promotionType = StringToPieceType(promotionType);
    moveData_.pieceType = StringToPieceType(pieceType);
}
// exf8=R+
void AttackPromotionMove::ProcessMove() {
    AttackMove::ProcessMove();
    PromotionMove::ProcessMove();
}

} // namespace pgn
