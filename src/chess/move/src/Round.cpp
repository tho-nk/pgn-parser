
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
    auto getMoveType = [](std::string_view type) {
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

    // Read one token starting at `begin` and report where that token ends.
    // Example: text=" e4 e5", begin=0 -> token="e4", end=3.
    auto readToken = [](std::string_view text, size_t begin, size_t &end) {
        begin = helper::GetNextNonSpace(text, begin);
        end = helper::GetNextSpace(text, begin);
        std::string token{text.substr(begin, end - begin)};
        helper::TrimSpace(token);
        return token;
    };

    // PGN separator token between white and black can appear as ., .., or ...
    // and may be attached to the move token (for example "...e5").
    auto isDotSeparator = [](std::string_view token) { return token == "." || token == ".." || token == "..."; };

    // Round format starts with "<index>.".
    // Example: "12. e4 e5" -> roundIndex_ = 12, moveText = " e4 e5".
    auto found = str.find(".");
    roundIndex_ = std::stoi(str.substr(0, found));
    const std::string_view moveText{str.data() + found + 1, str.size() - found - 1};

    // Parse optional comment at the round level before white move.
    // Example: "{must win} e4 e5" -> roundComment_ = "must win", indexBeginWhiteMove points to "e4".
    size_t indexBeginWhiteMove = 0;
    helper::GetComment(moveText, roundComment_, indexBeginWhiteMove);

    // Extract white move token and optional trailing white comment.
    // Example: "e4 {best by test} ... c5" -> whiteMove = "e4", whiteMoveComment = "best by test".
    size_t indexEndWhiteMove = 0;
    std::string whiteMove = readToken(moveText, indexBeginWhiteMove, indexEndWhiteMove);
    std::string whiteMoveComment;
    size_t indexBeginBlackMove = indexEndWhiteMove;
    helper::GetComment(moveText, whiteMoveComment, indexBeginBlackMove);

    // Ownership of parsed strings is transferred into the created move object.
    // Example: whiteMove="Nf3" and whiteMoveComment="develops knight" are moved into White move instance.
    whiteMove_.emplace(move_factory::CreateMove(getMoveType(whiteMove), Color::White, std::move(whiteMove),
                                                std::move(whiteMoveComment)));

    // Parse black move token. Supports all common PGN forms:
    // "1. e4 e5", "1. e4 ...e5", and "1. e4 ... e5".
    size_t indexEndBlackMove = 0;
    std::string blackMove = readToken(moveText, indexBeginBlackMove, indexEndBlackMove);

    // Skip separator-only tokens before the actual black SAN token.
    // Example: token="..." then next token="e5".
    while (isDotSeparator(blackMove)) {
        blackMove = readToken(moveText, indexEndBlackMove, indexEndBlackMove);
    }

    // Normalize attached separator notation like "...e5" -> "e5".
    helper::Remove3Dot(blackMove);

    // Parse optional black move comment and create the black move object.
    // Example: "c5 {Sicilian}" -> blackMove="c5", blackMoveComment="Sicilian".
    std::string blackMoveComment;
    size_t indexEnd = indexEndBlackMove;
    helper::GetComment(moveText, blackMoveComment, indexEnd);

    blackMove_.emplace(move_factory::CreateMove(getMoveType(blackMove), Color::Black, std::move(blackMove),
                                                std::move(blackMoveComment)));
}

void Round::Run() {
    // std::clog << "[THO][I] Round:=" << roundIndex_ << std::endl;
    // std::clog << "[THO][I] White move:" << std::endl;
    ProcessMove(*whiteMove_);
    // std::clog << pgn::Square::GetInstance().GetCurrentState() << std::endl;
    // std::clog << std::endl;
    // std::clog << "[THO][I] Black move" << std::endl;
    ProcessMove(*blackMove_);
    // std::clog << pgn::Square::GetInstance().GetCurrentState() << std::endl;

    // std::clog << "\n\n\n" << std::endl;
}
} // namespace pgn
