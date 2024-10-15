
#include "move/include/CastlingMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "piece/include/Square.hpp"

namespace mlp_ha {

CastlingMove::CastlingMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment)
    : Move(moveType, color, std::move(moveText), std::move(comment)) {}

void CastlingMove::ProcessMove() {
    // std::clog << "[THO][I] CastlingMove::ProcessMove" << std::endl;
    auto str = moveText_;
    // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
    helper::removeUnwantedChars(str);
    if (str == "O-O-O") {
        ToPosition toPositionKing{0, 2};
        FromPosition fromKing{0, 4};
        ToPosition toPositionRook{0, 3};
        FromPosition fromRook{0, 0};

        if (moveData_.color == Color::Black) {
            toPositionKing.row = 7;
            fromKing.row = 7;
            toPositionRook.row = 7;
            fromRook.row = 7;
        }
        mlp_ha::Square::GetInstance().MovePiece(fromKing, toPositionKing);
        mlp_ha::Square::GetInstance().MovePiece(fromRook, toPositionRook);

    } else {
        ToPosition toPositionKing{0, 6};
        FromPosition fromKing{0, 4};
        ToPosition toPositionRook{0, 5};
        FromPosition fromRook{0, 7};

        if (moveData_.color == Color::Black) {
            toPositionKing.row = 7;
            fromKing.row = 7;
            toPositionRook.row = 7;
            fromRook.row = 7;
        }

        mlp_ha::Square::GetInstance().MovePiece(fromKing, toPositionKing);
        mlp_ha::Square::GetInstance().MovePiece(fromRook, toPositionRook);
    }
}

} // namespace mlp_ha
