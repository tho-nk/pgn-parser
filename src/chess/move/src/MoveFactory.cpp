
#include "move/include/MoveFactory.hpp"
#include <iostream>

namespace pgn {

namespace move_factory {

MoveVariant CreateMove(const MoveType &moveType, const Color &color, std::string &&moveText, std::string &&comment) {
    switch (moveType) {
    case MoveType::BasicMove:
        return MoveVariant{std::in_place_type<BasicMove>, moveType, color, std::move(moveText), std::move(comment)};
    case MoveType::PromotionMove:
        return MoveVariant{std::in_place_type<PromotionMove>, moveType, color, std::move(moveText), std::move(comment)};
    case MoveType::CastlingMove:
        return MoveVariant{std::in_place_type<CastlingMove>, moveType, color, std::move(moveText), std::move(comment)};
    case MoveType::AttackMove:
        return MoveVariant{std::in_place_type<AttackMove>, moveType, color, std::move(moveText), std::move(comment)};
    case MoveType::AttackPromotionMove:
        return MoveVariant{std::in_place_type<AttackPromotionMove>, moveType, color, std::move(moveText),
                           std::move(comment)};
    default:
        return MoveVariant{std::monostate{}};
    }
}
}; // namespace move_factory
} // namespace pgn