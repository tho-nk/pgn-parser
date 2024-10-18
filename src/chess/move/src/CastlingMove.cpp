
#include "move/include/CastlingMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "piece/include/Square.hpp"

namespace mlp_ha {

CastlingMove::CastlingMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment)
    : Move(moveType, std::move(moveText), std::move(comment)) {
    moveData_ = std::make_unique<CastlingMoveData>(color);
    ComputeMoveData();
}

void CastlingMove::ComputeMoveData() {
    // std::clog << "[THO][I] CastlingMove::ProcessMove" << std::endl;
    auto str = moveText_;
    // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
    helper::removeUnwantedChars(str);
    if (str == "O-O-O") {
        FromPosition fromKing{0, 4};
        ToPosition toPositionKing{0, 2};
        FromPosition fromRook{0, 0};
        ToPosition toPositionRook{0, 3};

        if (moveData_->GetColor() == Color::Black) {
            toPositionKing.row = 7;
            fromKing.row = 7;
            toPositionRook.row = 7;
            fromRook.row = 7;
        }
        moveData_->SetKingFromPosition(fromKing);
        moveData_->SetKingToPosition(toPositionKing);
        moveData_->SetRookFromPosition(fromRook);
        moveData_->SetRookToPosition(toPositionRook);

    } else {
        FromPosition fromKing{0, 4};
        ToPosition toPositionKing{0, 6};
        FromPosition fromRook{0, 7};
        ToPosition toPositionRook{0, 5};

        if (moveData_->GetColor() == Color::Black) {
            toPositionKing.row = 7;
            fromKing.row = 7;
            toPositionRook.row = 7;
            fromRook.row = 7;
        }

        moveData_->SetKingFromPosition(fromKing);
        moveData_->SetKingToPosition(toPositionKing);
        moveData_->SetRookFromPosition(fromRook);
        moveData_->SetRookToPosition(toPositionRook);
    }
}

void CastlingMove::ProcessMove() { mlp_ha::Square::GetInstance().ProcessCastlingMove(moveData_.get()); }

} // namespace mlp_ha
