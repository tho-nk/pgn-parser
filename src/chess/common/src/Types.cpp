#include "common/include/Types.hpp"

namespace pgn {

bool AreOnFileOrRowOrDiagonal(const Position &p1, const Position &p2, const Position p3) {
    if (AreOnFile(p1, p2) && AreOnFile(p2, p3)) {
        return true;
    }
    if (AreOnRow(p1, p2) && AreOnRow(p2, p3)) {
        return true;
    }
    if (AreOnDiagonal(p1, p2) && AreOnDiagonal(p2, p3)) {
        return true;
    }
    return false;
}

bool AreOnFileOrRowOrDiagonal(const Position &p1, const Position &p2) {
    int dRow = std::abs(p1.row - p2.row);
    int dCol = std::abs(p1.col - p2.col);
    return (dRow == 0 || dCol == 0 || dRow == dCol);
}

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
        // std::cerr << "[THO][E] Error color type" << std::endl;
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
        // std::cerr << "[THO][E] Error PieceType" << std::endl;
        break;
    }
    return os;
}

PieceType StringToPieceType(const std::string &view) {
    switch (view[0]) {
    case 'K':
        return PieceType::King;
    case 'Q':
        return PieceType::Queen;
    case 'R':
        return PieceType::Rook;
    case 'B':
        return PieceType::Bishop;
    case 'N':
        return PieceType::Knight;
    case 'P':
        return PieceType::Pawn;
    default:
        return PieceType::Undefined;
    }
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
        // std::cerr << "[THO][E] Error Move" << std::endl;
        break;
    }
    return os;
}

} // namespace pgn
