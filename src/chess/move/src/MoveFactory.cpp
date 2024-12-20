
#include "move/include/MoveFactory.hpp"
#include "move/include/AttackMove.hpp"
#include "move/include/AttackPromotionMove.hpp"
#include "move/include/BasicMove.hpp"
#include "move/include/CastlingMove.hpp"
#include "move/include/PromotionMove.hpp"
#include <iostream>

namespace pgn {

namespace move_factory {

pgn::Move CreateMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment) {
    switch (moveType) {
    case MoveType::BasicMove:
        return pgn::Move(std::in_place_type<pgn::BasicMove>, moveType, color, std::move(moveText), std::move(comment));
    case MoveType::PromotionMove:
        return pgn::Move(std::in_place_type<pgn::PromotionMove>, moveType, color, std::move(moveText),
                         std::move(comment));
    case MoveType::CastlingMove:
        return pgn::Move(std::in_place_type<pgn::CastlingMove>, moveType, color, std::move(moveText),
                         std::move(comment));
    case MoveType::AttackMove:
        return pgn::Move(std::in_place_type<pgn::AttackMove>, moveType, color, std::move(moveText), std::move(comment));
    case MoveType::AttackPromotionMove:
        return pgn::Move(std::in_place_type<pgn::AttackPromotionMove>, moveType, color, std::move(moveText),
                         std::move(comment));
    default:
        return pgn::Move(std::in_place_type<pgn::MoveBase>, MoveType::Undefined, Color::Undefined, "", "");
    }
}
} // namespace move_factory
} // namespace pgn