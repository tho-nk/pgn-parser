#include "move/include/BasicMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "piece/include/Square.hpp"

#include <ranges>

namespace mlp_ha {

BasicMove::BasicMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {
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

void BasicMove::ProcessMove(Square *square) {
    const auto subPieces = square->GetPieceOfTypeAndColor(moveData_.pieceType, this->color_, moveData_.fromPosition);
    square->ProcessBasicMove(subPieces, this->color_, moveData_.toPosition, moveData_.fromPosition);
    square->MovePiece(moveData_.fromPosition, moveData_.toPosition);
}

} // namespace mlp_ha
