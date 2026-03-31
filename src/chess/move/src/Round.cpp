
#include "move/include/Round.hpp"
#include "common/include/ParsingHelper.hpp"
#include "move/include/MoveFactory.hpp"
#include "piece/include/Square.hpp"

namespace pgn {

Round::Round(const std::string &str) : roundIndex_(0) { ParseRoundText(str); }

void Round::ParseRoundText(const std::string &str) {
    // Classify SAN fragments into the concrete move type used by MoveFactory.
    // Example: "exd8=Q" -> AttackPromotionMove, "Nxe5" -> AttackMove, "O-O" -> CastlingMove,
    // "e8=Q" -> PromotionMove, "e4" -> BasicMove.
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

    // Round format starts with "<index>.".
    // Example: "12. e4 e5" -> roundIndex_ = 12, moveText = " e4 e5".
    auto found = str.find(".");
    roundIndex_ = std::stoi(str.substr(0, found));
    auto moveText = str.substr(found + 1);

    // Parse optional comment at the round level before white move.
    // Example: "{must win} e4 e5" -> roundComment_ = "must win", indexBeginWhiteMove points to "e4".
    size_t indexBeginWhiteMove = 0;
    helper::GetComment(moveText, roundComment_, indexBeginWhiteMove);

    // Extract white move token and optional trailing white comment.
    // Example: "e4 {best by test} ... c5" -> whiteMove = "e4", whiteMoveComment = "best by test".
    auto indexEndWhiteMove = helper::GetNextSpace(moveText, indexBeginWhiteMove);
    std::string whiteMove = moveText.substr(indexBeginWhiteMove, indexEndWhiteMove - indexBeginWhiteMove);
    helper::TrimSpace(whiteMove);
    std::string whiteMoveComment;
    size_t indexBeginBlackMove = indexEndWhiteMove;
    helper::GetComment(moveText, whiteMoveComment, indexBeginBlackMove);

    // Ownership of parsed strings is transferred into the created move object.
    // Example: whiteMove="Nf3" and whiteMoveComment="develops knight" are moved into White move instance.
    whiteMove_ = move_factory::CreateMove(getMoveType(whiteMove), Color::White, std::move(whiteMove),
                                          std::move(whiteMoveComment));

    // Parse black move token. Black text may start as "...e5" or "... e5".
    // Example: "...e5" arrives as one token, while "... e5" arrives as separator token then move token.
    // this works only if 1...MoveText but not for 1... MoveText
    auto indexEndBlackMove = helper::GetNextSpace(moveText, indexBeginBlackMove);
    std::string blackMove = moveText.substr(indexBeginBlackMove, indexEndBlackMove - indexBeginBlackMove);
    helper::TrimSpace(blackMove);
    auto ends_with = [](std::string const &value, std::string const &ending) {
        if (ending.size() > value.size())
            return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    };

    // If token is exactly round separator "...", advance once more to actual move.
    // Example: "1. e4 ... c5" -> first token is "...", next token extracted is "c5".
    if (ends_with(blackMove, "...")) {
        indexBeginBlackMove = indexEndBlackMove + 1;
        indexEndBlackMove = helper::GetNextSpace(moveText, indexBeginBlackMove);
        blackMove = moveText.substr(indexBeginBlackMove, indexEndBlackMove - indexBeginBlackMove);
        helper::TrimSpace(blackMove);
    } else {
        // Otherwise handle attached notation like "...e5".
        // Example: "1. e4 ...e5" -> Remove3Dot converts token from "...e5" to "e5".
        helper::Remove3Dot(blackMove);
    }

    // Parse optional black move comment and create the black move object.
    // Example: "c5 {Sicilian}" -> blackMove="c5", blackMoveComment="Sicilian".
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
