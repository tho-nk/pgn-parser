
#include "Move.hpp"
#include <regex>

namespace mlp_ha {

Move::Move(/*const BoardGame &boardGame, */ const std::string &move, const std::string &comment)
    : /*boardGame_(boardGame), */ move_(move), comment_(comment_) {}

} // namespace mlp_ha
