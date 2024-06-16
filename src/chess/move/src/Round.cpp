
#include "move/include/Round.hpp"
#include "common/include/ParsingHelper.hpp"
#include "move/include/MoveFactory.hpp"
#include "piece/include/Square.hpp"

namespace mlp_ha {

Round::Round(const std::string &str, Square *square) : roundIndex_(0), square_(square) { ParseRoundText(str); }

void Round::ParseRoundText(const std::string &str) {
    auto getMoveType = [&](std::string_view type) {
        if (type.empty()) {
            return MoveType::Undefined;
        }
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

void Round::Run() const {
    // std::clog << "[THO][I] Round:=" << roundIndex_ << std::endl;
    // std::clog << "[THO][I] White move:" << std::endl;
    whiteMove_->ProcessMove(square_);
    // square->Draw();
    // std::clog << std::endl;
    // std::clog << "[THO][I] Black move" << std::endl;
    blackMove_->ProcessMove(square_);

    // square->Draw();
    // std::clog << "\n\n\n" << std::endl;
}
} // namespace mlp_ha
