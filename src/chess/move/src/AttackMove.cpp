#include "move/include/AttackMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "piece/include/Square.hpp"

namespace mlp_ha {

AttackMove::AttackMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment)
    : Move(moveType, std::move(moveText), std::move(comment)) {
    moveData_ = std::make_unique<AttackMoveData>(color);
    ComputeMoveData();
}

void AttackMove::ComputeMoveData() {
    // std::clog << "[THO][I] AttackMove::ProcessMove" << std::endl;
    auto str = moveText_;
    // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
    helper::removeUnwantedChars(str);
    std::string_view remain(str.data(), str.length() - 3); // exclude 'x'
    std::string_view dest(str.data() + str.length() - 2, 2);
    moveData_->SetToPosition(Position{dest[1] - '1', dest[0] - 'a'});
    FromPosition fromPosition{-1, -1};
    std::string pieceType = "P";
    if (std::isupper(remain[0])) {
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
    } else {
        fromPosition.col = remain[0] - 'a';
    }
    moveData_->SetFromPosition(fromPosition);
    moveData_->SetPieceType(StringToPieceType(pieceType));
}

void AttackMove::ProcessMove() { mlp_ha::Square::GetInstance().ProcessAttackMove(moveData_.get()); }

} // namespace mlp_ha
