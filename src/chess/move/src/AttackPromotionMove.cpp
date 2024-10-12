#include "move/include/AttackPromotionMove.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "piece/include/Square.hpp"

namespace mlp_ha {

AttackPromotionMove::AttackPromotionMove(const MoveType &moveType, const Color &color, std::string moveText,
                                         std::string comment)
    : Move(moveType, color, moveText, comment) {}

// exf8=R+
void AttackPromotionMove::ProcessMove(Square *square) {
    try {
        // std::clog << "[THO][I] AttackPromotionMove::ProcessMove" << std::endl;
        auto str = moveText_;
        // std::clog << "[THO][I] Move:=" << moveText_ << std::endl;
        helper::removeUnwantedChars(str);

        auto foundA = str.find("x");
        auto foundP = str.find("=");
        // foundA + 3 == foundP
        std::string from = str.substr(0, foundA); // exclude 'x'
        std::string dest = str.substr(foundA + 1, foundP - foundA - 1);
        std::string promo = str.substr(foundP + 1);

        ToPosition toPosition{dest[1] - '1', dest[0] - 'a'};
        FromPosition fromPosition{-1, -1};
        std::string pieceType = "P"; // only for Pawn
        if (std::isupper(from[0])) {
            pieceType = from.at(0); // should be Pawn
            int index = 1;
            while (index < from.length()) {
                if (std::isdigit(from[index])) {
                    fromPosition.row = from[index] - '1';
                } else {
                    fromPosition.col = from[index] - 'a';
                }
                index++;
            }
        } else {
            fromPosition.col = from[0] - 'a';
        }

        auto promoType = StringToPieceType(promo);
        square->ProcessAttackPromotionMove(promoType, this->color_, fromPosition, toPosition);
    } catch (const MlpException &e) {
        // std::cerr << "[THO][E] AttackPromotionMove::ProcessMove invalid move : " << moveText_ << std::endl;
        std::cerr << "[THO][E] AttackPromotionMove::ProcessMove MlpException " << e.what() << std::endl;
        std::string message = "AttackPromotionMove::ProcessMove invalid move : " + moveText_ + ", " + e.what();
        throw MlpException(message.c_str());
    } catch (...) {
        std::cerr << "[THO][E] AttackPromotionMove::ProcessMove unkown exception" << std::endl;
    }
}

} // namespace mlp_ha
