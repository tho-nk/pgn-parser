#include "move/include/BasicMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "game/include/BoardGame.hpp"
#include <ranges>

namespace mlp_ha {

BasicMove::BasicMove(const MoveType &moveType, const Color &color, std::string moveText, std::string comment)
    : Move(moveType, color, moveText, comment) {}

void BasicMove::ProcessMove(const std::shared_ptr<BoardGame> &boardGame) {
    try {
        // std::clog << "[THO][I] BasicMove::ProcessMove" << std::endl;
        auto str = moveText_;
        // std::clog << "[THO][I] Move:= " << moveText_ << std::endl;
        helper::removeUnwantedChars(str);
        std::string_view remain(str.data(), str.length() - 2);
        std::string_view dest(str.data() + str.length() - 2, 2);
        ToPosition toPosition{dest[1] - '1', dest[0] - 'a'};
        FromPosition fromPosition{-1, -1};
        std::string pieceType = "P";
        if (!remain.empty()) {
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
        }
        auto type = StringToPieceType(pieceType);

        const auto subPieces = boardGame->GetPieceOfTypeAndColor(type, this->color_, fromPosition);
        boardGame->ProcessBasicMove(subPieces, this->color_, toPosition, fromPosition);
        boardGame->MovePiece(fromPosition, toPosition);
    } catch (const MlpException &e) {
        // std::cerr << "[THO][E] BasicMove::ProcessMove invalid move : " << moveText_ << std::endl;
        std::cerr << "[THO][E] BasicMove::ProcessMove MlpException " << e.what() << std::endl;
        std::string message = "BasicMove::ProcessMove invalid move : " + moveText_ + ", " + e.what();
        throw MlpException(message.c_str());
    } catch (...) {
        std::cerr << "[THO][E] BasicMove::ProcessMove unkown exception" << std::endl;
    }
}

} // namespace mlp_ha
