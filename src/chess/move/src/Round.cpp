
#include "move/include/Round.hpp"
#include "common/include/ParsingHelper.hpp"
#include "move/include/MoveFactory.hpp"
#include "piece/include/Square.hpp"

namespace pgn {

Round::Round(const std::string &str) : roundIndex_(0) { ParseRoundText(str); }

void Round::ParseRoundText(const std::string &str) {
    auto getMoveType = [&](std::string_view type) {
        if (type.empty()) {
            return MoveType::Undefined;
        }

        auto foundA = type.find("x");
        auto foundP = type.find("=");
        auto foundC = type.find("O");

        if (foundA != std::string::npos && foundP != std::string::npos) { // attack and promotion
            return MoveType::AttackPromotionMove;
        }

        if (foundA != std::string::npos) {
            // auto
            return MoveType::AttackMove;
        }
        if (foundC != std::string::npos) {
            return MoveType::CastlingMove;
        }
        if (foundP != std::string::npos) {
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

    whiteMove_ = move_factory::CreateMove(getMoveType(whiteMove), Color::White, std::move(whiteMove),
                                          std::move(whiteMoveComment));
    //  this works only if 1...MoveText but not for 1... MoveText
    auto indexEndBlackMove = helper::GetNextSpace(moveText, indexBeginBlackMove);
    std::string blackMove = moveText.substr(indexBeginBlackMove, indexEndBlackMove - indexBeginBlackMove);
    helper::TrimSpace(blackMove);
    auto ends_with = [](std::string const &value, std::string const &ending) {
        if (ending.size() > value.size())
            return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    };

    if (ends_with(blackMove, "...")) {
        indexBeginBlackMove = indexEndBlackMove + 1;
        indexEndBlackMove = helper::GetNextSpace(moveText, indexBeginBlackMove);
        blackMove = moveText.substr(indexBeginBlackMove, indexEndBlackMove - indexBeginBlackMove);
        helper::TrimSpace(blackMove);
    } else {
        helper::Remove3Dot(blackMove);
    }
    std::string blackMoveComment;
    size_t indexEnd = indexEndBlackMove;
    helper::GetComment(moveText, blackMoveComment, indexEnd);

    blackMove_ = move_factory::CreateMove(getMoveType(blackMove), Color::Black, std::move(blackMove),
                                          std::move(blackMoveComment));
}

void Round::Run() const {
    // std::clog << "[THO][I] Round:=" << roundIndex_ << std::endl;
    // std::clog << "[THO][I] White move:" << std::endl;
    whiteMove_->ProcessMove();
    // std::clog << pgn::Square::GetInstance().GetCurrentState() << std::endl;
    // std::clog << std::endl;
    // std::clog << "[THO][I] Black move" << std::endl;
    blackMove_->ProcessMove();
    // std::clog << pgn::Square::GetInstance().GetCurrentState() << std::endl;

    // std::clog << "\n\n\n" << std::endl;
}
} // namespace pgn
