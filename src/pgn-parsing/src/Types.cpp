#include "Types.hpp"

namespace mlp_ha {

std::ostream &operator<<(std::ostream &os, const Color color) {
    switch (color) {
    case Color::Black:
        os << "b";
        break;
    case Color::White:
        os << "w";
        break;
    case Color::Undefined:
        os << " ";
        break;
    default:
        std::cerr << "error type" << std::endl;
        break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const PieceType pieceType) {
    switch (pieceType) {
    case PieceType::King:
        os << "K";
        break;
    case PieceType::Queen:
        os << "Q";
        break;
    case PieceType::Rook:
        os << "R";
        break;
    case PieceType::Bishop:
        os << "B";
        break;
    case PieceType::Knight:
        os << "N";
        break;
    case PieceType::Pawn:
        os << "P";
        break;
    case PieceType::Undefined:
        os << " ";
        break;
    default:
        std::cerr << "error type" << std::endl;
        break;
    }
    return os;
}

PieceType StringToPieceType(std::string view) {
    if (view[0] == 'K') {
        return PieceType::King;
    }
    if (view[0] == 'Q') {
        return PieceType::Queen;
    }
    if (view[0] == 'R') {
        return PieceType::Rook;
    }
    if (view[0] == 'B') {
        return PieceType::Bishop;
    }
    if (view[0] == 'N') {
        return PieceType::Knight;
    }
    if (view[0] == 'P') {
        return PieceType::Pawn;
    }
    if (view[0] == ' ') {
        return PieceType::Undefined;
    }
    return PieceType::Undefined;
}

std::ostream &operator<<(std::ostream &os, const MoveType moveType) {
    switch (moveType) {
    case MoveType::BasicMove:
        os << "BasicMove";
        break;
    case MoveType::PromotionMove:
        os << "PromotionMove";
        break;
    case MoveType::CastlingMove:
        os << "CastlingMove";
        break;
    case MoveType::AttackMove:
        os << "AttackMove";
        break;
    default:
        std::cerr << "error type" << std::endl;
        break;
    }
    return os;
}

} // namespace mlp_ha
