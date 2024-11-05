
#include "move/include/CastlingMove.hpp"
#include "common/include/ParsingHelper.hpp"
#include "common/include/PgnException.hpp"
#include "piece/include/Square.hpp"

namespace pgn {

CastlingMove::CastlingMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment)
    : Move(moveType, color, std::move(moveText), std::move(comment)) {
    ComputeMoveData();
}

void CastlingMove::ComputeMoveData() {
    // std::clog << "[THO][I] CastlingMove::ProcessMove" << std::endl;
    auto str = moveText_;
    // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
    helper::removeUnwantedChars(str);

    if (str == "O-O-O") {
        moveData_.kingDesination = ToPosition{0, 2};
        moveData_.kingSource = FromPosition{0, 4};
        moveData_.rookDesination = ToPosition{0, 3};
        moveData_.rookSource = FromPosition{0, 0};

        if (moveData_.color == Color::Black) {
            moveData_.kingDesination.row = 7;
            moveData_.kingSource.row = 7;
            moveData_.rookDesination.row = 7;
            moveData_.rookSource.row = 7;
        }
    } else if (str == "O-O") {
        moveData_.kingDesination = ToPosition{0, 6};
        moveData_.kingSource = FromPosition{0, 4};
        moveData_.rookDesination = ToPosition{0, 5};
        moveData_.rookSource = FromPosition{0, 7};

        if (moveData_.color == Color::Black) {
            moveData_.kingDesination.row = 7;
            moveData_.kingSource.row = 7;
            moveData_.rookDesination.row = 7;
            moveData_.rookSource.row = 7;
        }
    }
}

void CastlingMove::ProcessMove() {
    pgn::Square::GetInstance().MovePiece(moveData_.kingSource, moveData_.kingDesination);
    pgn::Square::GetInstance().MovePiece(moveData_.rookSource, moveData_.rookDesination);
}

} // namespace pgn
