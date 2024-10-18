#include "move/include/PromotionMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "piece/include/Square.hpp"

namespace mlp_ha {

PromotionMove::PromotionMove(const MoveType &moveType, const Color &color, std::string &&moveText,
                             std::string &&comment)
    : Move(moveType, std::move(moveText), std::move(comment)) {
    moveData_ = std::make_unique<PromotionMoveData>(color);
    ComputeMoveData();
}

void PromotionMove::ComputeMoveData() {
    // std::clog << "[THO][I] PromotionMove::ProcessMove" << std::endl;
    auto str = moveText_;
    // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
    helper::removeUnwantedChars(str);

    std::string_view remain(str.data(), str.length() - 2);
    moveData_->SetToPosition(ToPosition{remain[1] - '1', remain[0] - 'a'});
    moveData_->SetFromPosition(FromPosition{moveData_->GetColor() == Color::White ? moveData_->GetToPosition().row - 1
                                                                                  : moveData_->GetToPosition().row + 1,
                                            moveData_->GetToPosition().col});
    std::string promotionType(str.data() + str.length() - 1, 1);
    moveData_->SetPromotionType(StringToPieceType(promotionType));
}

void PromotionMove::ProcessMove() { mlp_ha::Square::GetInstance().ProcessPromotionMove(moveData_.get()); }

} // namespace mlp_ha
