#include "move/include/BasicMove.hpp"
#include "common/include/ParsingHelper.hpp"
#include "common/include/PgnException.hpp"
#include "piece/include/Square.hpp"

#include <ranges>

namespace pgn {

BasicMove::BasicMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment)
    : Move(moveType, color, std::move(moveText), std::move(comment)) {
    ComputeMoveData();
}

void BasicMove::ComputeMoveData() {
    // std::clog << "[THO][I] BasicMove::ProcessMove" << std::endl;
    auto str = moveText_;
    // std::clog << "[THO][I] Move:= " << moveText_ << std::endl;
    helper::removeUnwantedChars(str);
    std::string_view remain(str.data(), str.length() - 2);
    std::string_view dest(str.data() + str.length() - 2, 2);
    moveData_.toPosition = Position{dest[1] - '1', dest[0] - 'a'};
    std::string pieceType = "P";
    if (!remain.empty()) {
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
    }
    moveData_.pieceType = StringToPieceType(pieceType);
}

void BasicMove::ProcessMove() {
    pgn::Square::GetInstance().ProcessBasicMove(moveData_);
    pgn::Square::GetInstance().MovePiece(moveData_.fromPosition, moveData_.toPosition);
}

} // namespace pgn
