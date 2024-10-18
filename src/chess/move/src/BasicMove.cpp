#include "move/include/BasicMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "piece/include/Square.hpp"

#include <ranges>

namespace mlp_ha {

BasicMove::BasicMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment)
    : Move(moveType, std::move(moveText), std::move(comment)) {
    moveData_ = std::make_unique<BasicMoveData>(color);
    ComputeMoveData();
}

void BasicMove::ComputeMoveData() {
    // std::clog << "[THO][I] BasicMove::ProcessMove" << std::endl;
    auto str = moveText_;
    // std::clog << "[THO][I] Move:= " << moveText_ << std::endl;
    helper::removeUnwantedChars(str);
    std::string_view remain(str.data(), str.length() - 2);
    std::string_view dest(str.data() + str.length() - 2, 2);
    moveData_->SetToPosition(Position{dest[1] - '1', dest[0] - 'a'});
    std::string pieceType = "P";
    FromPosition fromPosition{-1, -1};
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
    moveData_->SetFromPosition(fromPosition);
    moveData_->SetPieceType(StringToPieceType(pieceType));
}

void BasicMove::ProcessMove() { mlp_ha::Square::GetInstance().ProcessBasicMove(moveData_.get()); }

} // namespace mlp_ha
