
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
    std::cout << "Round::ParseRoundText str:=" << str << ", found:=" << found << std::endl;
    roundIndex_ = std::stoi(str.substr(0, found));
    auto moveText = str.substr(found + 1);

    size_t indexBeginFirstMove = 0;
    getComment(moveText, roundComment_, indexBeginFirstMove);
    std::cout << "roundComment_:=" << roundComment_ << std::endl;

    auto indexEndFirstMove = getNextSpace(moveText, indexBeginFirstMove);
    std::string firstMove = moveText.substr(indexBeginFirstMove, indexEndFirstMove - indexBeginFirstMove);
    std::cout << "firstMove := " << firstMove << std::endl;
    std::string firstMoveComment;
    size_t indexBeginSecondMove = indexEndFirstMove;
    getComment(moveText, firstMoveComment, indexBeginSecondMove);
    std::cout << "firstMoveComment:=" << firstMoveComment << std::endl;

    auto indexEndSecondMove = getNextSpace(moveText, indexBeginSecondMove);
    std::string secondMove = moveText.substr(indexBeginSecondMove, indexEndSecondMove - indexBeginSecondMove);
    std::cout << "secondMove := " << secondMove << std::endl;
    std::string secondMoveComment;
    size_t indexEnd = indexEndSecondMove;
    getComment(moveText, secondMoveComment, indexEnd);
    std::cout << "secondMoveComment:=" << secondMoveComment << std::endl;
}

} // namespace mlp_ha
