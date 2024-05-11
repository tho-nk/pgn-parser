#include "piece/include/Square.hpp"
#include <ranges>

namespace mlp_ha {

Square::Square() : whiteKingPosition_(0, 4), blackKingPosition_(7, 4), enPassant_(std::nullopt) {}
Square::~Square() {}

void Square::InitSquare() {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLUMNS; ++c) {
            pieces_[r][c].emplace<EmptyPiece>(Color::Undefined, Position{r, c});
        }
    }
    // Pawns
    for (auto c = 0; c < COLUMNS; ++c) {
        pieces_[1][c].emplace<Pawn>(Color::White, Position{1, c});
        pieces_[6][c].emplace<Pawn>(Color::Black, Position{6, c});
    }
    // Rooks
    pieces_[0][0].emplace<Rook>(Color::White, Position{0, 0});
    pieces_[0][7].emplace<Rook>(Color::White, Position{0, 7});
    pieces_[7][0].emplace<Rook>(Color::Black, Position{7, 0});
    pieces_[7][7].emplace<Rook>(Color::Black, Position{7, 7});

    // Knights
    pieces_[0][1].emplace<Knight>(Color::White, Position{0, 1});
    pieces_[0][6].emplace<Knight>(Color::White, Position{0, 6});
    pieces_[7][1].emplace<Knight>(Color::Black, Position{7, 1});
    pieces_[7][6].emplace<Knight>(Color::Black, Position{7, 6});

    // Bishops
    pieces_[0][2].emplace<Bishop>(Color::White, Position{0, 2});
    pieces_[0][5].emplace<Bishop>(Color::White, Position{0, 5});
    pieces_[7][2].emplace<Bishop>(Color::Black, Position{7, 2});
    pieces_[7][5].emplace<Bishop>(Color::Black, Position{7, 5});

    // Queens
    pieces_[0][3].emplace<Queen>(Color::White, Position{0, 3});
    pieces_[7][3].emplace<Queen>(Color::Black, Position{7, 3});

    // Kings
    pieces_[0][4].emplace<King>(Color::White, Position{0, 4});
    pieces_[7][4].emplace<King>(Color::Black, Position{7, 4});
}

void Square::Draw() {
    for (int r = ROWS - 1; r >= 0; --r) {
        for (auto c = 0; c < COLUMNS - 1; ++c) {
            std::visit(
                [](auto &&piece) {
                    std::cout << piece.GetDraw();
                    std::cout << "|";
                },
                pieces_[r][c]);
        }
        std::visit([](auto &&piece) { std::cout << piece.GetDraw(); }, pieces_[r][COLUMNS - 1]);
        std::cout << std::endl;
    }
}

PiecesReference Square::GetPieceOfTypeAndColor(const PieceType &pieceType, const Color &color,
                                               const FromPosition &fromPosition) {
    PiecesReference subPieces;
    if (fromPosition.IsValid()) {
        subPieces.push_back(std::ref(GetPieces()[fromPosition.row][fromPosition.col]));
    } else {
        auto arange = std::ranges::subrange(GetPieces().front().begin(), GetPieces().back().end());
        for (const auto &var : arange) {
            std::visit(
                [&](const auto &value) {
                    if (value.GetType() == pieceType && value.GetColor() == color) {
                        subPieces.push_back(std::ref(GetPieces()[value.GetPosition().row][value.GetPosition().col]));
                    }
                },
                var);
        }
    }
    return subPieces;
}

void Square::ProcessBasicMove(const PiecesReference &subPieces, const ToPosition &toPosition,
                              FromPosition &fromPosition) {
    if (fromPosition.IsValid()) {
        return;
    }

    bool isValid = false;
    for (auto &it : subPieces) {
        std::visit(
            [&](auto &&piece) {
                isValid = piece.IsValidBasicMove(shared_from_this(), toPosition); // IsValidMove(piece, toPosition);
                if (isValid) {
                    if ((fromPosition.row != -1 && fromPosition.row != piece.GetPosition().row) ||
                        (fromPosition.col != -1 && fromPosition.col != piece.GetPosition().col)) {
                        isValid = false;
                    } else {
                        fromPosition.row = piece.GetPosition().row;
                        fromPosition.col = piece.GetPosition().col;
                    }
                }
            },
            it.get());
        if (isValid)
            break;
    }
    if (!isValid) {
        std::cerr << "Square::ComputeFromPosition Error" << std::endl;
    }
}

void Square::MovePiece(const FromPosition &fromPosition, const ToPosition toPosition) {
    if (!fromPosition.IsValid() || !toPosition.IsValid()) {
        std::cout << "HELLO Invalid" << std::endl;
    }
    auto tmpF = Position(fromPosition.row, fromPosition.col);
    auto tmpT = Position(toPosition.row, toPosition.col);

    pieces_[fromPosition.row][fromPosition.col].swap(pieces_[toPosition.row][toPosition.col]);

    std::visit([&](auto &&piece) { piece.SetPosition(tmpF); }, pieces_[fromPosition.row][fromPosition.col]);

    std::visit([&](auto &&piece) { piece.SetPosition(tmpT); }, pieces_[toPosition.row][toPosition.col]);
}

void Square::AttackPiece(const FromPosition &fromPosition, const ToPosition toPosition) {

    auto tmpF = Position(fromPosition.row, fromPosition.col);
    auto tmpT = Position(toPosition.row, toPosition.col);
    pieces_[fromPosition.row][fromPosition.col].swap(pieces_[toPosition.row][toPosition.col]);

    std::visit([&](auto &&piece) { piece.SetPosition(tmpT); }, pieces_[toPosition.row][toPosition.col]);
    pieces_[fromPosition.row][fromPosition.col].emplace<EmptyPiece>(Color::Undefined, Position{tmpF.row, tmpF.col});

    if (enPassant_) {
        pieces_[enPassant_->row][enPassant_->col].emplace<EmptyPiece>(Color::Undefined, enPassant_.value());
        enPassant_ = std::nullopt;
    }
}

void Square::ProcessAttackMove(const PiecesReference &subPieces, const ToPosition &toPosition,
                               FromPosition &fromPosition) {

    if (fromPosition.IsValid()) {
        return;
    }

    bool isValid = false;
    for (auto &it : subPieces) {
        std::visit(
            [&](auto &&piece) {
                // if ((fromPosition.row != -1 && piece.GetPosition().row == fromPosition.row) ||
                //     (fromPosition.col != -1 && piece.GetPosition().col == fromPosition.col)) {

                isValid = piece.IsValidAttackMove(shared_from_this(), toPosition);
                if (isValid) {
                    if ((fromPosition.row != -1 && fromPosition.row != piece.GetPosition().row) ||
                        (fromPosition.col != -1 && fromPosition.col != piece.GetPosition().col)) {
                        isValid = false;
                    } else {
                        fromPosition.row = piece.GetPosition().row;
                        fromPosition.col = piece.GetPosition().col;
                    }
                }
                // }
            },
            it.get());
        if (isValid)
            break;
    }
    if (!isValid) {
        std::cerr << "Square::ComputeFromPosition Error" << std::endl;
    }
}

void Square::ProcessPromotionMove(const PieceType &pieceType, const Color &color, const FromPosition &fromPosition,
                                  const ToPosition &toPosition) {

    Piece newPiece;
    switch (pieceType) {
    // case PieceType::King:
    //     break;
    case PieceType::Queen:
        newPiece.emplace<Queen>(color, toPosition);
        break;
    case PieceType::Rook:
        newPiece.emplace<Queen>(color, toPosition);
        break;
    case PieceType::Bishop:
        newPiece.emplace<Queen>(color, toPosition);
        break;
    case PieceType::Knight:
        newPiece.emplace<Queen>(color, toPosition);
        break;
    case PieceType::Pawn:
        newPiece.emplace<Queen>(color, toPosition);
        break;
    case PieceType::Undefined:
        break;
    default:
        break;
    }
    pieces_[toPosition.row][toPosition.col].swap(newPiece);
    std::visit([&](auto &&piece) { piece.SetPosition(Position{toPosition.row, toPosition.col}); },
               pieces_[toPosition.row][toPosition.col]);
    pieces_[fromPosition.row][fromPosition.col].emplace<EmptyPiece>(Color::Undefined,
                                                                    Position{fromPosition.row, fromPosition.col});
}
} // namespace mlp_ha