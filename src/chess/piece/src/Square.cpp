#include "piece/include/Square.hpp"
#include "common/include/MlpException.hpp"
#include "common/include/ParsingHelper.hpp"
#include "move/include/Round.hpp"
#include <ranges>

namespace mlp_ha {

void Square::Init() {
    for (int r = 2; r < ROWS - 2; ++r) {
        for (int c = 0; c < COLUMNS; ++c) {
            pieces_[r][c].emplace<EmptyPiece>(Color::Undefined, Position{r, c}, this);
        }
    }

    // Pawns
    for (auto c = 0; c < COLUMNS; ++c) {
        pieces_[1][c].emplace<Pawn>(Color::White, Position{1, c}, this);
        pieces_[6][c].emplace<Pawn>(Color::Black, Position{6, c}, this);
    }
    // Rooks
    pieces_[0][0].emplace<Rook>(Color::White, Position{0, 0}, this);
    pieces_[0][7].emplace<Rook>(Color::White, Position{0, 7}, this);
    pieces_[7][0].emplace<Rook>(Color::Black, Position{7, 0}, this);
    pieces_[7][7].emplace<Rook>(Color::Black, Position{7, 7}, this);

    // Knights
    pieces_[0][1].emplace<Knight>(Color::White, Position{0, 1}, this);
    pieces_[0][6].emplace<Knight>(Color::White, Position{0, 6}, this);
    pieces_[7][1].emplace<Knight>(Color::Black, Position{7, 1}, this);
    pieces_[7][6].emplace<Knight>(Color::Black, Position{7, 6}, this);

    // Bishops
    pieces_[0][2].emplace<Bishop>(Color::White, Position{0, 2}, this);
    pieces_[0][5].emplace<Bishop>(Color::White, Position{0, 5}, this);
    pieces_[7][2].emplace<Bishop>(Color::Black, Position{7, 2}, this);
    pieces_[7][5].emplace<Bishop>(Color::Black, Position{7, 5}, this);

    // Queens
    pieces_[0][3].emplace<Queen>(Color::White, Position{0, 3}, this);
    pieces_[7][3].emplace<Queen>(Color::Black, Position{7, 3}, this);

    // Kings
    pieces_[0][4].emplace<King>(Color::White, Position{0, 4}, this);
    pieces_[7][4].emplace<King>(Color::Black, Position{7, 4}, this);
}

void Square::Run() {
    for (const auto &round : rounds_) {
        round.Run();
    }
}

void Square::LoadPGN() {
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
        rounds_.emplace_back(roundText, this);
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
    rounds_.emplace_back(roundText, this);
}

std::string Square::GetCurrentState() const noexcept {
    std::ostringstream ss;
    for (int r = ROWS - 1; r >= 0; --r) {
        for (auto c = 0; c < COLUMNS - 1; ++c) {
            std::visit(
                [&](auto &&piece) {
                    ss << piece.GetDraw();
                    ss << "|";
                },
                GetPieces()[r][c]);
        }
        std::visit([&](auto &&piece) { ss << piece.GetDraw(); }, GetPieces()[r][COLUMNS - 1]);
        ss << "\n";
    }
    return ss.str();
}

PiecesReference Square::GetPieceOfTypeAndColor(const PieceType &pieceType, const Color &color,
                                               const FromPosition &fromPosition) const noexcept {
    PiecesReference subPieces;
    if (fromPosition.IsValid()) {
        subPieces.push_back(std::ref(GetPieces()[fromPosition.row][fromPosition.col]));
    } else {
        auto arange = std::ranges::subrange(GetPieces().front().begin(), GetPieces().back().end());
        for (const auto &var : arange) {
            std::visit(
                [&](auto &&value) {
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
    const auto &kings = GetPieceOfTypeAndColor(PieceType::King, color, Position{});
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
        throw MlpException("Square::VerifyIfKingBeingCheck Error");
    }

    auto FindNextNonEmpty = [this](const Position &start, int dr, int dc) {
        Position pos = {start.row + dr, start.col + dc};
        while (pos.IsValid()) {
            if (!IsEmptyAt({pos.row, pos.col})) {
                break;
            }
            pos.row += dr;
            pos.col += dc;
        }
        return pos;
    };

    Position possibleOpponent = FindNextNonEmpty(piecePosition, dr[index], dc[index]);
    Position possibleObstacle = FindNextNonEmpty(piecePosition, -dr[index], -dc[index]);
    bool kingChecked = false;

    if (possibleObstacle.IsValid() && possibleObstacle == kingPosition && possibleOpponent.IsValid()) {
        std::visit(
            [&](auto &&opponent) {
                if (opponent.GetColor() != Color::Undefined && opponent.GetColor() != pieceColor) {
                    kingChecked = opponent.IsValidBasicMove(kingPosition, piecePosition);
                }
            },
            GetPieces()[possibleOpponent.row][possibleOpponent.col]);
    }

    return kingChecked;
}

void Square::MovePiece(const FromPosition &fromPosition, const ToPosition toPosition) {
    if (!fromPosition.IsValid() || !toPosition.IsValid()) {
        std::cerr << "[THO][E] Invalid position" << std::endl;
        throw MlpException("Error while moving : Invalid position");
    }
    auto tmpF = Position(fromPosition.row, fromPosition.col);
    auto tmpT = Position(toPosition.row, toPosition.col);

    pieces_[fromPosition.row][fromPosition.col].swap(pieces_[toPosition.row][toPosition.col]);

    std::visit([&](auto &&piece) { piece.SetPosition(tmpF); }, pieces_[fromPosition.row][fromPosition.col]);

    std::visit([&](auto &&piece) { piece.SetPosition(tmpT); }, pieces_[toPosition.row][toPosition.col]);
}

void Square::AttackPiece(const FromPosition &fromPosition, const ToPosition toPosition) {
    if (!fromPosition.IsValid() || !toPosition.IsValid()) {
        std::cerr << "[THO][E] Invalid position" << std::endl;
        throw MlpException("Error while attacking : Invalid position");
    }
    auto tmpF = Position(fromPosition.row, fromPosition.col);
    auto tmpT = Position(toPosition.row, toPosition.col);
    pieces_[fromPosition.row][fromPosition.col].swap(pieces_[toPosition.row][toPosition.col]);

    std::visit([&](auto &&piece) { piece.SetPosition(tmpT); }, pieces_[toPosition.row][toPosition.col]);

    pieces_[fromPosition.row][fromPosition.col].emplace<EmptyPiece>(Color::Undefined, Position{tmpF.row, tmpF.col},
                                                                    this);

    if (enPassant_) {
        pieces_[enPassant_->row][enPassant_->col].emplace<EmptyPiece>(Color::Undefined, enPassant_.value(), this);
        enPassant_ = std::nullopt;
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

void Square::ProcessPromotionMove(const PieceType &promotionType, const Color &color, const FromPosition &fromPosition,
                                  const ToPosition &toPosition) {

    switch (promotionType) {
    case PieceType::Queen:
        pieces_[toPosition.row][toPosition.col].emplace<Queen>(color, toPosition, this);
        break;
    case PieceType::Rook:
        pieces_[toPosition.row][toPosition.col].emplace<Rook>(color, toPosition, this);
        break;
    case PieceType::Bishop:
        pieces_[toPosition.row][toPosition.col].emplace<Bishop>(color, toPosition, this);
        break;
    case PieceType::Knight:
        pieces_[toPosition.row][toPosition.col].emplace<Knight>(color, toPosition, this);
        break;
    case PieceType::Pawn:
    default:
        std::cerr << "[THO][E] Square::ProcessPromotionMove" << std::endl;
        throw MlpException("Square::ProcessPromotionMove Cannot promote to undefined piece");
        break;
    }
    pieces_[fromPosition.row][fromPosition.col].emplace<EmptyPiece>(Color::Undefined,
                                                                    Position{fromPosition.row, fromPosition.col}, this);
}

} // namespace mlp_ha