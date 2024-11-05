#include "move/include/AttackMove.hpp"
#include "common/include/ParsingHelper.hpp"
#include "common/include/PgnException.hpp"
#include "piece/include/Square.hpp"

namespace pgn {

AttackMove::AttackMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment)
    : Move(moveType, color, std::move(moveText), std::move(comment)) {
    ComputeMoveData();
}

void AttackMove::ComputeMoveData() {
    // std::clog << "[THO][I] AttackMove::ProcessMove" << std::endl;
    auto str = moveText_;
    // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
    helper::removeUnwantedChars(str);
    std::string_view remain(str.data(), str.length() - 3); // exclude 'x'
    std::string_view dest(str.data() + str.length() - 2, 2);
    moveData_.toPosition = Position{dest[1] - '1', dest[0] - 'a'};
    std::string pieceType = "P";
    if (std::isupper(remain[0])) {
        pieceType = remain.at(0);
        int index = 1;
        while (index < remain.length()) {
            if (std::isdigit(remain[index])) {
                moveData_.fromPosition.row = remain[index] - '1';
            } else {
                moveData_.fromPosition.col = remain[index] - 'a';
            }
            index++;
        }
    } else {
        moveData_.fromPosition.col = remain[0] - 'a';
    }

    moveData_.pieceType = StringToPieceType(pieceType);
}

void AttackMove::ProcessMove() {
    pgn::Square::GetInstance().ProcessAttackMove(moveData_);
    pgn::Square::GetInstance().AttackPiece(moveData_.fromPosition, moveData_.toPosition);
}

} // namespace pgn
