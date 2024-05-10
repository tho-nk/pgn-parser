
#include "move/include/Round.hpp"
#include "common/include/ParsingHelper.hpp"
#include "move/include/MoveFactory.hpp"

namespace mlp_ha {

Round::Round(const std::string &str) : roundIndex_(0) { ParseRoundText(str); }

void Round::ParseRoundText(const std::string &str) {
    auto getMoveType = [&](std::string_view type) {
        auto found = type.find("x");
        if (found != std::string::npos) {
            return MoveType::AttackMove;
        }
        found = type.find("O");
        if (found != std::string::npos) {
            return MoveType::CastlingMove;
        }
        found = type.find("=");
        if (found != std::string::npos) {
            return MoveType::PromotionMove;
        }
        return MoveType::BasicMove;
    };

    auto found = str.find(".");
    roundIndex_ = std::stoi(str.substr(0, found));
    auto moveText = str.substr(found + 1);

    size_t indexBeginWhiteMove = 0;
    helper::GetComment(moveText, roundComment_, indexBeginWhiteMove);

    auto indexEndWhiteMove = helper::GetNextSpace(moveText, indexBeginWhiteMove);
    std::string whiteMove = moveText.substr(indexBeginWhiteMove, indexEndWhiteMove - indexBeginWhiteMove);
    helper::TrimSpace(whiteMove);
    std::string whiteMoveComment;
    size_t indexBeginBlackMove = indexEndWhiteMove;
    helper::GetComment(moveText, whiteMoveComment, indexBeginBlackMove);

    whiteMove_ = move_factory::CreateMove(getMoveType(whiteMove), Color::White, whiteMove, whiteMoveComment);

    auto indexEndBlackMove = helper::GetNextSpace(moveText, indexBeginBlackMove);
    std::string blackMove = moveText.substr(indexBeginBlackMove, indexEndBlackMove - indexBeginBlackMove);
    helper::Remove3Dot(blackMove);
    helper::TrimSpace(blackMove);
    std::string blackMoveComment;
    size_t indexEnd = indexEndBlackMove;
    helper::GetComment(moveText, blackMoveComment, indexEnd);

    blackMove_ = move_factory::CreateMove(getMoveType(blackMove), Color::Black, blackMove, blackMoveComment);
}

} // namespace mlp_ha
