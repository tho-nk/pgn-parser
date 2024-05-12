#include "move/include/PromotionMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "game/include/BoardGame.hpp"

namespace mlp_ha {

PromotionMove::PromotionMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {}

void PromotionMove::ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {
    try {
        // std::clog << "[THO][I] PromotionMove::ProcessMove" << std::endl;
        auto str = moveText_;
        // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
        helper::removeUnwantedChars(str);

        std::string_view remain(str.data(), str.length() - 2);
        ToPosition toPosition{remain[1] - '1', remain[0] - 'a'};
        FromPosition fromPosition{toPosition.row - 1, toPosition.col};
        std::string pieceType(str.data() + str.length() - 1, 1);
        auto type = StringToPieceType(pieceType);
        boardGame->ProcessPromotionMove(type, this->color_, fromPosition, toPosition);
    } catch (const MlpException &e) {
        std::cerr << "[THO][E] PromotionMove::ProcessMove invalid move : " << moveText_ << std::endl;
        std::string message = "PromotionMove::ProcessMove invalid move : " + moveText_ + ", " + e.what();
        throw MlpException(message.c_str());
    } catch (...) {
        std::cerr << "[THO][E] PromotionMove::ProcessMove unkown exception" << std::endl;
    }
}

} // namespace mlp_ha
