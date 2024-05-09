
#include "Round.hpp"
#include "ParsingHelper.hpp"
#include <regex>

namespace mlp_ha {

Round::Round(/*const BoardGame &boardGame, */ const std::string &str) : /*boardGame_(boardGame), */ roundIndex_(0) {
    // extract comment, move from str
    // std::cout << "Round::Round str:=" << str << std::endl;
    ParseRoundText(str);
}
void Round::ParseRoundText(const std::string &str) {
    auto found = str.find(".");
    // std::cout << "Round::ParseRoundText str:=" << str << ", found:=" << found << std::endl;
    roundIndex_ = std::stoi(str.substr(0, found));
    auto moveText = str.substr(found + 1);

    size_t indexBeginWhiteMove = 0;
    helper::GetComment(moveText, roundComment_, indexBeginWhiteMove);
    // std::cout << "roundComment_:=" << roundComment_ << std::endl;

    auto indexEndWhiteMove = helper::GetNextSpace(moveText, indexBeginWhiteMove);
    std::string whiteMove = moveText.substr(indexBeginWhiteMove, indexEndWhiteMove - indexBeginWhiteMove);
    helper::TrimSpace(whiteMove);
    // std::cout << "whiteMove := " << whiteMove << std::endl;
    std::string whiteMoveComment;
    size_t indexBeginBlackMove = indexEndWhiteMove;
    helper::GetComment(moveText, whiteMoveComment, indexBeginBlackMove);
    // std::cout << "whiteMoveComment:=" << whiteMoveComment << std::endl;

    whiteMove_.SetMove(whiteMove, whiteMoveComment);

    auto indexEndBlackMove = helper::GetNextSpace(moveText, indexBeginBlackMove);
    std::string blackMove = moveText.substr(indexBeginBlackMove, indexEndBlackMove - indexBeginBlackMove);
    helper::Remove3Dot(blackMove);
    helper::TrimSpace(blackMove);
    // std::cout << "blackMove := " << blackMove << std::endl;
    std::string blackMoveComment;
    size_t indexEnd = indexEndBlackMove;
    helper::GetComment(moveText, blackMoveComment, indexEnd);
    // std::cout << "blackMoveComment:=" << blackMoveComment << std::endl;

    blackMove_.SetMove(blackMove, blackMoveComment);
}

} // namespace mlp_ha
