#include "piece/include/Square.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "move/include/Round.hpp"
#include <ranges>

namespace mlp_ha {

void Square::InitSquare() {
    pieces_.reserve(ROWS);
    std::vector<Piece> rowPiece;
    rowPiece.reserve(COLUMNS);
    // first row
    rowPiece.emplace_back(std::in_place_type<Rook>, Color::White, Position{0, 0}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Knight>, Color::White, Position{0, 1}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Bishop>, Color::White, Position{0, 2}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Queen>, Color::White, Position{0, 3}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<King>, Color::White, Position{0, 4}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Bishop>, Color::White, Position{0, 5}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Knight>, Color::White, Position{0, 6}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Rook>, Color::White, Position{0, 7}, shared_from_this());
    pieces_.push_back(std::move(rowPiece));

    // second row
    rowPiece.clear();
    rowPiece.reserve(COLUMNS);

    for (auto c = 0; c < COLUMNS; ++c) {
        rowPiece.emplace_back(std::in_place_type<Pawn>, Color::White, Position{1, c}, shared_from_this());
    }
    pieces_.push_back(std::move(rowPiece));

    for (int r = 2; r < ROWS - 2; ++r) {
        rowPiece.clear();
        rowPiece.reserve(COLUMNS);

        for (int c = 0; c < COLUMNS; ++c) {
            rowPiece.emplace_back(std::in_place_type<EmptyPiece>, Color::Undefined, Position{r, c}, shared_from_this());
        }
        pieces_.push_back(std::move(rowPiece));
    }

    // before last row
    rowPiece.clear();
    rowPiece.reserve(COLUMNS);

    for (auto c = 0; c < COLUMNS; ++c) {
        rowPiece.emplace_back(std::in_place_type<Pawn>, Color::Black, Position{6, c}, shared_from_this());
    }
    pieces_.push_back(std::move(rowPiece));

    // last row
    rowPiece.clear();
    rowPiece.reserve(COLUMNS);

    rowPiece.emplace_back(std::in_place_type<Rook>, Color::Black, Position{ROWS - 1, 0}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Knight>, Color::Black, Position{ROWS - 1, 1}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Bishop>, Color::Black, Position{ROWS - 1, 2}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Queen>, Color::Black, Position{ROWS - 1, 3}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<King>, Color::Black, Position{ROWS - 1, 4}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Bishop>, Color::Black, Position{ROWS - 1, 5}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Knight>, Color::Black, Position{ROWS - 1, 6}, shared_from_this());
    rowPiece.emplace_back(std::in_place_type<Rook>, Color::Black, Position{ROWS - 1, 7}, shared_from_this());
    pieces_.push_back(std::move(rowPiece));
}

void Square::Run() {
    for (const auto &round : rounds_) {
        round.Run();
    }
}

void Square::LoadData() {
    auto parsingHelper = helper::ParseFile(filePath_);
    if (parsingHelper.roundQueue.empty()) {
        // std::clog << "[THO][I] start game" << std::endl;
        return;
    }
    rounds_.reserve(parsingHelper.roundQueue.size() + 1);
    int round = 1;
    while (!parsingHelper.roundQueue.empty()) {
        auto roundText = parsingHelper.roundQueue.front();
        parsingHelper.roundQueue.pop();
        rounds_.emplace_back(roundText, shared_from_this());
        round++;
    }

    if (!helper::IsBalanced(parsingHelper.lastRun)) {
        std::cerr << "[THO][E] File format error : " << parsingHelper.lastRun << "\n  Cannot process round := " << round
                  << std::endl;
    }
    auto found = parsingHelper.lastRun.find("1-0");
    if (found == std::string::npos) {
        found = parsingHelper.lastRun.find("0-1");
    }
    if (found == std::string::npos) {
        found = parsingHelper.lastRun.find("1/2-1/2");
    }
    if (found == std::string::npos) {
        // std::cerr << "[THO][E] Game result error" << std::endl;
    }
    auto roundText = parsingHelper.lastRun.substr(0, found);
    // std::clog << "[THO][I] roundText:=" << roundText << std::endl;
    rounds_.emplace_back(roundText, shared_from_this());
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
        for (const auto &file : GetPieces()) {
            for (const auto &var : file) {
                std::visit(
                    [&](const auto &value) {
                        if (value.GetType() == pieceType && value.GetColor() == color) {
                            subPieces.push_back(
                                std::ref(GetPieces()[value.GetPosition().row][value.GetPosition().col]));
                        }
                    },
                    var);
            }
        }
    }
    return subPieces;
}

Position Square::GetKingPosition(const Color &color) const {
    const auto kings = GetPieceOfTypeAndColor(PieceType::King, color, Position{});
    return std::get<King>(kings.at(0).get()).GetPosition();
}

void Square::ProcessBasicMove(const PiecesReference &subPieces, const Color &color, const ToPosition &toPosition,
                              FromPosition &fromPosition) {
    if (fromPosition.IsValid()) {
        return;
    }
    bool isValid = false;
    const auto &kingPosition = GetKingPosition(color);
    for (auto &it : subPieces) {
        std::visit(
            [&](auto &&piece) {
                isValid = piece.IsValidBasicMove(toPosition);
                ValidateMove(kingPosition, piece.GetPosition(), toPosition, piece.GetColor(), isValid, fromPosition);
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
                        kingChecked = opponent.IsValidBasicMove(kingPosition, piecePosition);
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
        std::cerr << "[THO][E] Square::MovePiece unkown exception" << std::endl;
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
        std::cerr << "[THO][E] Square::AttackPiece unkown exception" << std::endl;
    }
}

void Square::ProcessAttackMove(const PiecesReference &subPieces, const Color &color, const ToPosition &toPosition,
                               FromPosition &fromPosition) {

    if (fromPosition.IsValid()) {
        return;
    }
    bool isValid = false;
    const auto &kingPosition = GetKingPosition(color);
    for (auto &it : subPieces) {
        std::visit(
            [&](auto &&piece) {
                isValid = piece.IsValidAttackMove(toPosition);
                ValidateMove(kingPosition, piece.GetPosition(), toPosition, piece.GetColor(), isValid, fromPosition);
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

void Square::ValidateMove(const Position &kingPosition, const Position &piecePosition, const ToPosition &toPosition,
                          const Color &pieceColor, bool &isValid, FromPosition &fromPosition) {
    if (isValid) {
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
        newPiece.emplace<Queen>(color, toPosition, shared_from_this());
        break;
    case PieceType::Rook:
        newPiece.emplace<Queen>(color, toPosition, shared_from_this());
        break;
    case PieceType::Bishop:
        newPiece.emplace<Queen>(color, toPosition, shared_from_this());
        break;
    case PieceType::Knight:
        newPiece.emplace<Queen>(color, toPosition, shared_from_this());
        break;
    case PieceType::Pawn:
        newPiece.emplace<Queen>(color, toPosition, shared_from_this());
        break;
    default:
        std::cerr << "[THO][E] Square::ProcessPromotionMove" << std::endl;
        throw MlpException("Square::ProcessPromotionMove Cannot promote to undefined piece");
        break;
    }
    pieces_[toPosition.row][toPosition.col].swap(newPiece);
    std::visit([&](auto &&piece) { piece.SetPosition(Position{toPosition.row, toPosition.col}); },
               pieces_[toPosition.row][toPosition.col]);
    pieces_[fromPosition.row][fromPosition.col].emplace<EmptyPiece>(
        Color::Undefined, Position{fromPosition.row, fromPosition.col}, shared_from_this());
}
} // namespace mlp_ha