
#include "move/include/CastlingMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "game/include/BoardGame.hpp"

namespace mlp_ha {

CastlingMove::CastlingMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {}

void CastlingMove::ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {
    try {
        // std::clog << "[THO][I] CastlingMove::ProcessMove" << std::endl;
        auto str = moveText_;
        // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
        helper::removeUnwantedChars(str);
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
    } catch (const MlpException &e) {
        // std::cerr << "[THO][E] CastlingMove::ProcessMove invalid move : " << moveText_ << std::endl;
        std::cerr << "[THO][E] CastlingMove::ProcessMove MlpException " << e.what() << std::endl;
        std::string message = "CastlingMove::ProcessMove invalid move : " + moveText_ + ", " + e.what();
        throw MlpException(message.c_str());
    } catch (...) {
        std::cerr << "[THO][E] CastlingMove::ProcessMove unkown exception" << std::endl;
    }
}

} // namespace mlp_ha
