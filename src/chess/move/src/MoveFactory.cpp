
#include "move/include/MoveFactory.hpp"
#include "move/include/AttackMove.hpp"
#include "move/include/BasicMove.hpp"
#include "move/include/CastlingMove.hpp"
#include "move/include/PromotionMove.hpp"
#include <iostream>

namespace mlp_ha {

namespace move_factory {

std::unique_ptr<Move> CreateMove(const MoveType &moveType, const Color &color, std::string moveText,
                                 std::string comment) {
    switch (moveType) {
    case MoveType::BasicMove:
        return std::make_unique<BasicMove>(moveType, color, moveText, comment);
        break;
    case MoveType::PromotionMove:
        return std::make_unique<PromotionMove>(moveType, color, moveText, comment);
        break;
    case MoveType::CastlingMove:
        return std::make_unique<CastlingMove>(moveType, color, moveText, comment);
        break;
    case MoveType::AttackMove:
        return std::make_unique<AttackMove>(moveType, color, moveText, comment);
        break;
    default:
        // std::clog << "[THO][I] Last move - result" << std::endl;
        return std::make_unique<Move>(MoveType::Undefined, Color::Undefined, "", "");
        break;
    }
}
}; // namespace move_factory
} // namespace mlp_ha