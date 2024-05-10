#include "CastlingMove.hpp"
#include "BoardGame.hpp"
#include "ParsingHelper.hpp"

namespace mlp_ha {

CastlingMove::CastlingMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {}

void CastlingMove::ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {
    std::cout << "CastlingMove::ProcessMove" << std::endl;
    auto str = moveText_;
    std::cout << moveText_ << std::endl;
    helper::removeUnwantedChars(str);
    // O-O-O left
    if (str == "O-O-O") {
        ToPosition toPositionKing{0, 2};
        FromPosition fromKing{0, 4};
        ToPosition toPositionRook{0, 3};
        FromPosition fromRook{0, 0};

        if (this->color_ == Color::Black) {
            toPositionKing.row = 7;
            fromKing.row = 7;
            toPositionRook.row = 7;
            fromRook.row = 7;
        }
        boardGame->MovePiece(fromKing, toPositionKing);
        boardGame->MovePiece(fromRook, toPositionRook);

    } else {
        ToPosition toPositionKing{0, 6};
        FromPosition fromKing{0, 4};
        ToPosition toPositionRook{0, 5};
        FromPosition fromRook{0, 7};

        if (this->color_ == Color::Black) {
            toPositionKing.row = 7;
            fromKing.row = 7;
            toPositionRook.row = 7;
            fromRook.row = 7;
        }

        boardGame->MovePiece(fromKing, toPositionKing);
        boardGame->MovePiece(fromRook, toPositionRook);
    }
}

} // namespace mlp_ha
