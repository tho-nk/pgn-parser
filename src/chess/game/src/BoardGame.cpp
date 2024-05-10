#include "game/include/BoardGame.hpp"
#include "common/include/ParsingHelper.hpp"
#include <algorithm>

namespace mlp_ha {
BoardGame::BoardGame(const std::filesystem::path &filePath)
    : filePath_(filePath), whiteKingPosition_(0, 4), blackKingPosition_(7, 4), enPassant_(std::nullopt) {
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

void BoardGame::Run() {
    for (const auto &round : rounds_) {
        auto roundIndex = round.GetRoundIndex();
        const auto &whiteMove = round.GetWhiteMove();
        const auto &blackMove = round.GetBlackMove();
        std::cout << "round:=" << roundIndex << std::endl;
        std::cout << "White move" << std::endl;
        whiteMove->ProcessMove(shared_from_this());
        Draw();
        std::cout << std::endl;
        std::cout << "Black move" << std::endl;
        blackMove->ProcessMove(shared_from_this());
        Draw();
        std::cout << "\n\n\n" << std::endl;
    }
}

void BoardGame::Draw() {
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

void BoardGame::LoadData() {
    auto roundQueue = helper::ParseFile(filePath_);
    while (!roundQueue.empty()) {
        auto roundText = roundQueue.front();
        roundQueue.pop();
        rounds_.emplace_back(roundText);
    }
}

void BoardGame::ProcessBasicMove(const PiecesReference &subPieces, const ToPosition &toPosition,
                                 FromPosition &fromPosition) {
    if (fromPosition.IsValid()) {
        return;
    }

    bool isValid = false;
    for (auto &it : subPieces) {
        std::visit(
            [&](auto &&piece) {
                isValid = IsValidMove(piece, toPosition);
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
        std::cerr << "BoardGame::ComputeFromPosition Error" << std::endl;
    }
}

bool BoardGame::IsValidMove(const King &king, const ToPosition &toPosition) {
    auto canQueenMove = [&]() { return true; }();
    return canQueenMove;
}
bool BoardGame::IsValidMove(const Queen &queen, const ToPosition &toPosition) {
    // basiclly there is only one queen, no need to check but for security
    auto canQueenMove = [&]() { return true; };
    if (canQueenMove()) {
        // TODO
        if (IsKingChedked()) {
            return false;
        }
        return true;
    }
    return false;
}

bool BoardGame::IsValidMove(const Rook &rook, const ToPosition &toPosition) {
    auto canRookMove = [&]() {
        // Rook can move only along rows or columns
        if (rook.GetPosition().row != toPosition.row && rook.GetPosition().col != toPosition.col) {
            return false; // Not moving along a row or column
        }

        if (rook.GetPosition().row == toPosition.row) {
            int start_col = std::min(rook.GetPosition().col, toPosition.col);
            int end_col = std::max(rook.GetPosition().col, toPosition.col);
            for (int c = start_col + 1; c < end_col; ++c) {
                if (!std::holds_alternative<EmptyPiece>(pieces_[toPosition.row][c])) {
                    return false;
                }
            }
        } else {
            int start_row = std::min(rook.GetPosition().row, rook.GetPosition().row);
            int end_row = std::max(rook.GetPosition().row, rook.GetPosition().row);
            for (int r = start_row + 1; r < end_row; ++r) {
                if (!std::holds_alternative<EmptyPiece>(pieces_[r][toPosition.col])) {
                    return false;
                }
            }
        }
        return true; // No pieces obstruct movement
    };
    if (canRookMove()) {
        // TODO
        if (IsKingChedked()) {
            return false;
        }
        return true;
    }
    return false;
}

bool BoardGame::IsValidMove(const Bishop &bishop, const ToPosition &toPosition) {
    auto canBishopMove = [&]() {
        // Calculate the absolute differences in row and column
        int dR = std::abs(bishop.GetPosition().row - toPosition.row);
        int dC = std::abs(bishop.GetPosition().col - toPosition.col);

        // A bishop can move only diagonally, so the dR and dC must be equal
        // and the positions must not be the same:  && p1 != p2; not need here (the move was confirmed)
        // there is only on bishop can move to that position, no need to check obstacle
        return (dR == dC);
    };
    if (canBishopMove()) {
        // TODO
        if (IsKingChedked()) {
            return false;
        }
        return true;
    }
    return false;
}

bool BoardGame::IsValidMove(const Knight &knight, const ToPosition &toPosition) {
    auto canKnightMove = [&]() {
        // Possible moves for a knight
        constexpr int dr[] = {-2, -1, 1, 2, 2, 1, -1, -2};
        constexpr int dc[] = {-1, -2, -2, -1, 1, 2, 2, 1};

        // Check all possible moves from knight
        for (int i = 0; i < 8; ++i) {
            int newR = knight.GetPosition().row + dr[i];
            int newC = knight.GetPosition().col + dc[i];
            // Check if the move leads to toPosition
            if (newR == toPosition.row && newC == toPosition.col) {
                return true;
            }
        }
        // If none of the moves lead to toPosition
        return false;
    };
    if (canKnightMove()) {
        // TODO
        if (IsKingChedked()) {
            return false;
        }
        return true;
    }
    return false;
}

bool BoardGame::IsValidMove(const Pawn &pawn, const ToPosition &toPosition) {
    if (pawn.GetColor() == Color::White) {
        // no need to check Check. there only one pawn can move
        if ((pawn.GetPosition().col != toPosition.col) || (pawn.GetPosition().row >= toPosition.row)) {
            return false;
        }
        if (pawn.GetPosition().row == (toPosition.row - 1)) {
            return true;
        }
        for (int r = pawn.GetPosition().row + 1; r < toPosition.row; ++r) {
            if (!std::holds_alternative<EmptyPiece>(pieces_[r][toPosition.col])) {
                return false;
            }
        }
        return true;
    } else if (pawn.GetColor() == Color::Black) {
        // no need to check Check. there only one pawn can move
        if ((pawn.GetPosition().col != toPosition.col) || (pawn.GetPosition().row <= toPosition.row)) {
            return false;
        }
        if (pawn.GetPosition().row == (toPosition.row + 1)) {
            return true;
        }
        for (int r = pawn.GetPosition().row - 1; r > toPosition.row; --r) {
            if (!std::holds_alternative<EmptyPiece>(pieces_[r][toPosition.col])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool BoardGame::IsValidMove(const EmptyPiece &empty, const ToPosition &toPosition) { return false; }

void BoardGame::MovePiece(const FromPosition &fromPosition, const ToPosition toPosition) {
    auto tmpF = Position(fromPosition.row, fromPosition.col);
    auto tmpT = Position(toPosition.row, toPosition.col);

    pieces_[fromPosition.row][fromPosition.col].swap(pieces_[toPosition.row][toPosition.col]);

    std::visit([&](auto &&piece) { piece.SetPosition(tmpF); }, pieces_[fromPosition.row][fromPosition.col]);

    std::visit([&](auto &&piece) { piece.SetPosition(tmpT); }, pieces_[toPosition.row][toPosition.col]);
}

void BoardGame::AttackPiece(const FromPosition &fromPosition, const ToPosition toPosition) {

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

void BoardGame::ProcessAttackMove(const PiecesReference &subPieces, const ToPosition &toPosition,
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

                isValid = IsValidAttackMove(piece, toPosition);
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
        std::cerr << "BoardGame::ComputeFromPosition Error" << std::endl;
    }
}

bool BoardGame::IsValidAttackMove(const King &king, const ToPosition &toPosition) {
    return IsValidMove(king, toPosition);
}
bool BoardGame::IsValidAttackMove(const Queen &queen, const ToPosition &toPosition) {
    return IsValidMove(queen, toPosition);
}
bool BoardGame::IsValidAttackMove(const Rook &rook, const ToPosition &toPosition) {
    return IsValidMove(rook, toPosition);
}
bool BoardGame::IsValidAttackMove(const Bishop &bishop, const ToPosition &toPosition) {
    return IsValidMove(bishop, toPosition);
}
bool BoardGame::IsValidAttackMove(const Knight &knight, const ToPosition &toPosition) {
    return IsValidMove(knight, toPosition);
}
bool BoardGame::IsValidAttackMove(const EmptyPiece &empty, const ToPosition &toPosition) {
    return IsValidMove(empty, toPosition);
}

bool BoardGame::IsValidAttackMove(const Pawn &pawn, const ToPosition &toPosition) {
    if (pawn.GetColor() == Color::White) {
        // en passant
        if (std::holds_alternative<EmptyPiece>(pieces_[toPosition.row][toPosition.col])) {
            SetEnPassant(Position{toPosition.row - 1, toPosition.col});
        }
        // no need to check Check. there only one pawn can move
        if (std::abs(pawn.GetPosition().col - toPosition.col) != 1) {
            return false;
        }
        if (pawn.GetPosition().row + 1 != toPosition.row) {
            return false;
        }
        return true;
    } else if (pawn.GetColor() == Color::Black) {
        if (std::holds_alternative<EmptyPiece>(pieces_[toPosition.row][toPosition.col])) {
            SetEnPassant(Position{toPosition.row + 1, toPosition.col});
        }
        // no need to check Check. there only one pawn can move
        if (std::abs(pawn.GetPosition().col - toPosition.col) != 1) {
            return false;
        }
        if (pawn.GetPosition().row - 1 != toPosition.row) {
            return false;
        }
        return true;
    }
    return false;
}

void BoardGame::ProcessPromotionMove(const PieceType &pieceType, const Color &color, const FromPosition &fromPosition,
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
