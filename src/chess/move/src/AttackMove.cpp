#include "move/include/AttackMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "game/include/BoardGame.hpp"

namespace mlp_ha {

AttackMove::AttackMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {}

void AttackMove::ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {
    try {
        // std::clog << "[THO][I] AttackMove::ProcessMove" << std::endl;
        auto str = moveText_;
        // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
        helper::removeUnwantedChars(str);
        std::string_view remain(str.data(), str.length() - 3); // exclude 'x'
        std::string_view dest(str.data() + str.length() - 2, 2);
        ToPosition toPosition{dest[1] - '1', dest[0] - 'a'};
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

        auto type = StringToPieceType(pieceType);
        const auto subPieces = boardGame->GetPieceOfTypeAndColor(type, this->color_, fromPosition);
        boardGame->ProcessAttackMove(subPieces, this->color_, toPosition, fromPosition);
        boardGame->AttackPiece(fromPosition, toPosition);
    } catch (const MlpException &e) {
        // std::cerr << "[THO][E] AttackMove::ProcessMove invalid move : " << moveText_ << std::endl;
        std::cerr << "[THO][E] AttackMove::ProcessMove MlpException " << e.what() << std::endl;
        std::string message = "AttackMove::ProcessMove invalid move : " + moveText_ + ", " + e.what();
        throw MlpException(message.c_str());
    } catch (...) {
        std::cerr << "[THO][E] AttackMove::ProcessMove unkown exception" << std::endl;
    }
}

} // namespace mlp_ha
