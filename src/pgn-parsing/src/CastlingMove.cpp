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
        ToPosition toPositionRook{0, 2};
        FromPosition fromRook{0, 0};

        if (this->color_ == Color::Black) {
            toPositionKing.row = 7;
            fromKing.row = 7;
            toPositionRook.row = 7;
            fromRook.row = 7;
        }

        Piece king(CreatePiece("K", this->color_, toPositionKing));
        Piece rook(CreatePiece("R", this->color_, toPositionRook));
        std::visit([&](const auto &piece) { boardGame->ProcessMove(piece, fromKing); }, king);
        std::visit([&](const auto &piece) { boardGame->ProcessMove(piece, fromRook); }, rook);
    } else {
        ToPosition toPositionKing{0, 2};
        FromPosition fromKing{0, 4};
        ToPosition toPositionRook{0, 2};
        FromPosition fromRook{0, 0};

        if (this->color_ == Color::Black) {
            toPositionKing.row = 7;
            fromKing.row = 7;
            toPositionRook.row = 7;
            fromRook.row = 7;
        }

        Piece king(CreatePiece("K", this->color_, toPositionKing));
        Piece rook(CreatePiece("R", this->color_, toPositionRook));
        std::visit([&](const auto &piece) { boardGame->ProcessMove(piece, fromKing); }, king);
        std::visit([&](const auto &piece) { boardGame->ProcessMove(piece, fromRook); }, rook);
    }
}

} // namespace mlp_ha
