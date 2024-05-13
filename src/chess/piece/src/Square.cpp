#include "piece/include/Square.hpp"
#include "common/include/MlpException.hpp"

#include <ranges>

namespace mlp_ha {

Square::Square() : enPassant_(std::nullopt) {}
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

std::string Square::GetCurrentState() const {
    std::ostringstream ss;
    for (int r = ROWS - 1; r >= 0; --r) {
        for (auto c = 0; c < COLUMNS - 1; ++c) {
            std::visit(
                [&](auto &&piece) {
                    ss << piece.GetDraw();
                    ss << "|";
                },
                pieces_[r][c]);
        }
        std::visit([&](auto &&piece) { ss << piece.GetDraw(); }, pieces_[r][COLUMNS - 1]);
        ss << "\n";
    }
    return ss.str();
}

PiecesReference Square::GetPieceOfTypeAndColor(const PieceType &pieceType, const Color &color,
                                               const FromPosition &fromPosition) const {
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

Position Square::GetKingPosition(const Color &color) const {
    const auto kings = GetPieceOfTypeAndColor(PieceType::King, color, Position{});
    return std::get<King>(kings.at(0).get()).GetPosition();
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
                isValid = piece.IsValidBasicMove(shared_from_this(), toPosition);
                ValidateMove(piece.GetPosition(), toPosition, piece.GetColor(), isValid, fromPosition);
            },
            it.get());
        if (isValid)
            break;
    }
    if (!isValid) {
        std::cerr << "[THO][E] Square::ProcessBasicMove Error" << std::endl;
        throw MlpException("Square::ProcessBasicMove Error");
    }
}

bool Square::VerifyIfKingBeingCheck(const Position &piecePosition, const Color &pieceColor,
                                    const Position &kingPosition) {
    bool kingChecked = false;
    constexpr int dr[] = {-1, 0, 1, 1, 1, 0, -1, -1};
    constexpr int dc[] = {-1, -1, -1, 0, 1, 1, 1, 0};

    int drow = piecePosition.row - kingPosition.row;
    int dcol = piecePosition.col - kingPosition.col;

    // Normalize the change in row and column to -1, 0, or 1
    if (drow != 0) {
        drow /= std::abs(drow);
    }
    if (dcol != 0) {
        dcol /= std::abs(dcol);
    }
    // Search for the direction in the directional arrays
    int index = 0;
    for (; index < 8; ++index) {
        if (dr[index] == drow && dc[index] == dcol) {
            break;
        }
    }
    if (index >= 8) {
        std::cerr << "[THO][E] Cannot find direction" << std::endl;
    }
    Position possibleOpponent{piecePosition.row + dr[index], piecePosition.col + dc[index]};
    while (possibleOpponent.IsValid()) {
        if (!std::holds_alternative<EmptyPiece>(GetPieces()[possibleOpponent.row][possibleOpponent.col])) {
            break;
        }
        possibleOpponent.row = possibleOpponent.row + dr[index];
        possibleOpponent.col = possibleOpponent.col + dc[index];
    }

    Position possibleObstacle{piecePosition.row - dr[index], piecePosition.col - dc[index]};
    while (possibleObstacle.IsValid()) {
        if (!std::holds_alternative<EmptyPiece>(GetPieces()[possibleObstacle.row][possibleObstacle.col])) {
            break;
        }
        possibleObstacle.row = possibleObstacle.row - dr[index];
        possibleObstacle.col = possibleObstacle.col - dc[index];
    }
    if (possibleObstacle.IsValid()) {
        if (possibleObstacle.row == kingPosition.row && possibleObstacle.col == kingPosition.col) {
            if (possibleOpponent.IsValid()) {
                std::visit(
                    [&](auto &&opponent) {
                        if (opponent.GetColor() == Color::Undefined) {
                            return;
                        }
                        if (opponent.GetColor() == pieceColor) {
                            return;
                        }
                        kingChecked = opponent.IsValidBasicMove(shared_from_this(), kingPosition, piecePosition);
                    },
                    GetPieces()[possibleOpponent.row][possibleOpponent.col]);
            }
        }
    }
    return kingChecked;
}

void Square::MovePiece(const FromPosition &fromPosition, const ToPosition toPosition) {
    try {
        if (!fromPosition.IsValid() || !toPosition.IsValid()) {
            std::cerr << "[THO][E] Invalid position" << std::endl;
            throw MlpException("Error while moving : Invalid position");
        }
        auto tmpF = Position(fromPosition.row, fromPosition.col);
        auto tmpT = Position(toPosition.row, toPosition.col);

        pieces_[fromPosition.row][fromPosition.col].swap(pieces_[toPosition.row][toPosition.col]);

        std::visit([&](auto &&piece) { piece.SetPosition(tmpF); }, pieces_[fromPosition.row][fromPosition.col]);

        std::visit([&](auto &&piece) { piece.SetPosition(tmpT); }, pieces_[toPosition.row][toPosition.col]);

        // Update King position here
    } catch (const MlpException &e) {
        throw;
    } catch (...) {
    }
}

void Square::AttackPiece(const FromPosition &fromPosition, const ToPosition toPosition) {
    try {
        if (!fromPosition.IsValid() || !toPosition.IsValid()) {
            std::cerr << "[THO][E] Invalid position" << std::endl;
            throw MlpException("Error while attacking : Invalid position");
        }
        auto tmpF = Position(fromPosition.row, fromPosition.col);
        auto tmpT = Position(toPosition.row, toPosition.col);
        pieces_[fromPosition.row][fromPosition.col].swap(pieces_[toPosition.row][toPosition.col]);

        std::visit([&](auto &&piece) { piece.SetPosition(tmpT); }, pieces_[toPosition.row][toPosition.col]);
        pieces_[fromPosition.row][fromPosition.col].emplace<EmptyPiece>(Color::Undefined, Position{tmpF.row, tmpF.col});

        if (enPassant_) {
            pieces_[enPassant_->row][enPassant_->col].emplace<EmptyPiece>(Color::Undefined, enPassant_.value());
            enPassant_ = std::nullopt;
        }
        // Update King position here
    } catch (const MlpException &e) {
        throw;
    } catch (...) {
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
                isValid = piece.IsValidAttackMove(shared_from_this(), toPosition);
                ValidateMove(piece.GetPosition(), toPosition, piece.GetColor(), isValid, fromPosition);
            },
            it.get());
        if (isValid)
            break;
    }
    if (!isValid) {
        std::cerr << "[THO][E] Square::ProcessAttackMove Error" << std::endl;
        throw MlpException("Square::ProcessAttackMove Error");
    }
}

void Square::ValidateMove(const Position &piecePosition, const ToPosition &toPosition, const Color &pieceColor,
                          bool &isValid, FromPosition &fromPosition) {
    if (isValid) {
        const auto &kingPosition = GetKingPosition(pieceColor);
        if (!(!AreOnFileOrRowOrDiagonal(kingPosition, piecePosition) ||
              (AreOnFileOrRowOrDiagonal(kingPosition, piecePosition, toPosition)))) {
            if (VerifyIfKingBeingCheck(piecePosition, pieceColor, kingPosition)) {
                isValid = false;
            }
        }
    }
    if (isValid) {
        if ((fromPosition.row != -1 && fromPosition.row != piecePosition.row) ||
            (fromPosition.col != -1 && fromPosition.col != piecePosition.col)) {
            isValid = false;
        } else {
            fromPosition.row = piecePosition.row;
            fromPosition.col = piecePosition.col;
        }
    }
}

void Square::ProcessPromotionMove(const PieceType &pieceType, const Color &color, const FromPosition &fromPosition,
                                  const ToPosition &toPosition) {

    Piece newPiece;
    switch (pieceType) {
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
    default:
        std::cerr << "[THO][E] Square::ProcessPromotionMove" << std::endl;
        throw MlpException("Square::ProcessPromotionMove Cannot promote to undefined piece");
        break;
    }
    pieces_[toPosition.row][toPosition.col].swap(newPiece);
    std::visit([&](auto &&piece) { piece.SetPosition(Position{toPosition.row, toPosition.col}); },
               pieces_[toPosition.row][toPosition.col]);
    pieces_[fromPosition.row][fromPosition.col].emplace<EmptyPiece>(Color::Undefined,
                                                                    Position{fromPosition.row, fromPosition.col});
}
} // namespace mlp_ha