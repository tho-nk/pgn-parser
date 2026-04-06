#include "piece/include/Square.hpp"
#include "common/include/ParsingHelper.hpp"
#include "common/include/PgnException.hpp"
#include "move/include/Round.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <ranges>
#include <string_view>

namespace pgn {

int Square::ColorToIndex_(const Color &color) noexcept {
    switch (color) {
    case Color::White:
        return 0;
    case Color::Black:
        return 1;
    case Color::Undefined:
    default:
        return -1;
    }
}

int Square::PieceTypeToIndex_(const PieceType &pieceType) noexcept {
    switch (pieceType) {
    case PieceType::King:
        return 0;
    case PieceType::Queen:
        return 1;
    case PieceType::Rook:
        return 2;
    case PieceType::Bishop:
        return 3;
    case PieceType::Knight:
        return 4;
    case PieceType::Pawn:
        return 5;
    case PieceType::Undefined:
    default:
        return -1;
    }
}

void Square::AddPieceToCache_(const Color &color, const PieceType &pieceType, const Position &position) {
    const auto colorIndex = ColorToIndex_(color);
    const auto typeIndex = PieceTypeToIndex_(pieceType);
    if (colorIndex < 0 || typeIndex < 0 || !position.IsValid()) {
        return;
    }

    auto &positions = piecePositions_[colorIndex][typeIndex];
    // Append only if not already present; this keeps each piece position unique in the cache.
    for (size_t i = 0; i < positions.count; ++i) {
        if (positions.data[i] == position) {
            return;
        }
    }

    if (positions.count >= positions.data.size()) {
        std::cerr << "[THO][E] Position cache overflow" << std::endl;
        throw PgnException("Position cache overflow");
    }

    // Store at the first free slot, then advance the logical size.
    positions.data[positions.count++] = position;

    if (pieceType == PieceType::King) {
        kingPositions_[colorIndex] = position;
    }
}

void Square::RemovePieceFromCache_(const Color &color, const PieceType &pieceType, const Position &position) {
    const auto colorIndex = ColorToIndex_(color);
    const auto typeIndex = PieceTypeToIndex_(pieceType);
    if (colorIndex < 0 || typeIndex < 0 || !position.IsValid()) {
        return;
    }

    auto &positions = piecePositions_[colorIndex][typeIndex];
    // O(1) erase pattern: replace removed slot with last active element and shrink count.
    for (size_t i = 0; i < positions.count; ++i) {
        if (positions.data[i] == position) {
            positions.data[i] = positions.data[positions.count - 1];
            --positions.count;
            break;
        }
    }

    if (pieceType == PieceType::King && kingPositions_[colorIndex] == position) {
        kingPositions_[colorIndex].Reset();
    }
}

void Square::MovePieceInCache_(const Color &color, const PieceType &pieceType, const Position &fromPosition,
                               const Position &toPosition) {
    const auto colorIndex = ColorToIndex_(color);
    const auto typeIndex = PieceTypeToIndex_(pieceType);
    if (colorIndex < 0 || typeIndex < 0 || !fromPosition.IsValid() || !toPosition.IsValid()) {
        return;
    }

    auto &positions = piecePositions_[colorIndex][typeIndex];
    // In-place relocate of one cached position; avoids remove+add churn.
    for (size_t i = 0; i < positions.count; ++i) {
        if (positions.data[i] == fromPosition) {
            positions.data[i] = toPosition;
            if (pieceType == PieceType::King) {
                kingPositions_[colorIndex] = toPosition;
            }
            return;
        }
    }

    std::cerr << "[THO][E] Cache desync: source position not found in MovePieceInCache_" << std::endl;
    throw PgnException("Cache desync: source position not found in MovePieceInCache_");
}

Square::PositionList Square::GetPiecePositions_(const PieceType &pieceType, const Color &color,
                                                const FromPosition &fromPosition) const noexcept {
    PositionList result;
    if (fromPosition.IsValid()) {
        result.data[0] = fromPosition;
        result.count = 1;
        return result;
    }

    const auto colorIndex = ColorToIndex_(color);
    const auto typeIndex = PieceTypeToIndex_(pieceType);
    if (colorIndex < 0 || typeIndex < 0) {
        return result;
    }

    return piecePositions_[colorIndex][typeIndex];
}

void Square::RebuildPositionCache_() {
    for (auto &types : piecePositions_) {
        for (auto &positions : types) {
            positions.count = 0;
        }
    }
    for (auto &kingPosition : kingPositions_) {
        kingPosition.Reset();
    }

    for (const auto &file : GetPieces()) {
        for (const auto &var : file) {
            std::visit(
                [&](const auto &piece) { AddPieceToCache_(piece.GetColor(), piece.GetType(), piece.GetPosition()); },
                var);
        }
    }
}

void Square::ResetState_() {
    for (int r = 2; r < ROWS - 2; ++r) {
        for (int c = 0; c < COLUMNS; ++c) {
            pieces_[r][c].emplace<EmptyPiece>(Position{r, c});
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

    enPassant_.Reset();
    RebuildPositionCache_();
}

void Square::Run() {
    for (const auto &round : rounds_) {
        round.Run();
    }
}

void Square::LoadPGN(const std::filesystem::path &filePath) {
    ResetState_();
    rounds_.clear();

    auto parsingHelper = helper::ParseFile(filePath);
    int round = 1;
    if (!parsingHelper.roundQueue.empty()) {
        rounds_.reserve(parsingHelper.roundQueue.size() + 1);
        while (!parsingHelper.roundQueue.empty()) {
            auto roundText = parsingHelper.roundQueue.front();
            parsingHelper.roundQueue.pop();
            rounds_.emplace_back(roundText);
            round++;
        }
    }
    if (!parsingHelper.lastRun.empty()) {
        if (!helper::IsBalanced(parsingHelper.lastRun)) {
            std::cerr << "[THO][E] File format error : " << parsingHelper.lastRun
                      << "\n  Cannot process round := " << round << std::endl;
        }
        constexpr std::array<std::string_view, 3> resultTokens = {"1-0", "0-1", "1/2-1/2"};
        auto found = std::string::npos;
        for (const auto token : resultTokens) {
            found = parsingHelper.lastRun.find(token);
            if (found != std::string::npos) {
                break;
            }
        }
        if (found == std::string::npos) {
            // std::cerr << "[THO][E] Game result error" << std::endl;
        }
        auto roundText = parsingHelper.lastRun.substr(0, found);
        helper::TrimSpace(roundText);
        // std::clog << "[THO][I] roundText:=" << roundText << std::endl;
        if (!roundText.empty()) {
            rounds_.emplace_back(roundText);
        }
    }
}

std::string Square::GetCurrentState() const noexcept {
    std::ostringstream ss;
    for (int r = ROWS - 1; r >= 0; --r) {
        for (auto c = 0; c < COLUMNS - 1; ++c) {
            std::visit(
                [&](const auto &piece) {
                    ss << piece.GetDraw();
                    ss << "|";
                },
                GetPieces()[r][c]);
        }
        std::visit([&](const auto &piece) { ss << piece.GetDraw(); }, GetPieces()[r][COLUMNS - 1]);
        ss << "\n";
    }
    return ss.str();
}

Position Square::GetKingPosition_(const Color &color) const {
    const auto colorIndex = ColorToIndex_(color);
    if (colorIndex < 0) {
        return Position{};
    }
    return kingPositions_[colorIndex];
}

void Square::ProcessBasicMove(MoveData &moveData) const {
    if (moveData.fromPosition.IsValid()) {
        return;
    }
    bool isValid = false;
    const auto subPieces = GetPiecePositions_(moveData.pieceType, moveData.color, moveData.fromPosition);

    const auto &kingPosition = GetKingPosition_(moveData.color);
    for (size_t i = 0; i < subPieces.count; ++i) {
        const auto &pieceRef = GetPiecesAt(subPieces.data[i]);
        std::visit(
            [&](const auto &piece) {
                isValid = piece.IsValidBasicMove(moveData.toPosition);
                ValidateMove_(kingPosition, piece.GetPosition(), moveData, isValid);
            },
            pieceRef);
        if (isValid)
            break;
    }
    if (!isValid) {
        std::cerr << "[THO][E] Square::ProcessBasicMove Error while validating a move" << std::endl;
        throw PgnException("Square::ProcessBasicMove Error while validating a move");
    }
}

void Square::ProcessAttackMove(MoveData &moveData) const {
    if (moveData.fromPosition.IsValid()) {
        return;
    }
    bool isValid = false;
    const auto subPieces = GetPiecePositions_(moveData.pieceType, moveData.color, moveData.fromPosition);
    const auto &kingPosition = GetKingPosition_(moveData.color);
    for (size_t i = 0; i < subPieces.count; ++i) {
        const auto &pieceRef = GetPiecesAt(subPieces.data[i]);
        std::visit(
            [&](const auto &piece) {
                isValid = piece.IsValidAttackMove(moveData.toPosition);
                ValidateMove_(kingPosition, piece.GetPosition(), moveData, isValid);
            },
            pieceRef);
        if (isValid)
            break;
    }
    if (!isValid) {
        std::cerr << "[THO][E] Square::ProcessAttackMove Error while validating an AttackMove" << std::endl;
        throw PgnException("Square::ProcessAttackMove Error while validating an AttackMove");
    }
}

void Square::ProcessPromotionMove(const PieceType &promotionType, const Color &color, const FromPosition &fromPosition,
                                  const ToPosition &toPosition) {
    std::visit(
        [&](const auto &piece) {
            if (piece.GetType() == PieceType::Pawn && piece.GetColor() == color) {
                RemovePieceFromCache_(piece.GetColor(), piece.GetType(), piece.GetPosition());
            }
        },
        GetPiecesAt(fromPosition));

    std::visit(
        [&](const auto &piece) {
            if (piece.GetType() == PieceType::Pawn && piece.GetColor() == color) {
                RemovePieceFromCache_(piece.GetColor(), piece.GetType(), piece.GetPosition());
            }
        },
        GetPiecesAt(toPosition));

    switch (promotionType) {
    case PieceType::Queen:
        GetPiecesAt(toPosition).emplace<Queen>(color, toPosition);
        break;
    case PieceType::Rook:
        GetPiecesAt(toPosition).emplace<Rook>(color, toPosition);
        break;
    case PieceType::Bishop:
        GetPiecesAt(toPosition).emplace<Bishop>(color, toPosition);
        break;
    case PieceType::Knight:
        GetPiecesAt(toPosition).emplace<Knight>(color, toPosition);
        break;
    case PieceType::Pawn:
    default:
        std::cerr << "[THO][E] Square::ProcessPromotionMove Error while promoting" << std::endl;
        throw PgnException("Square::ProcessPromotionMove Error while promoting");
    }
    AddPieceToCache_(color, promotionType, toPosition);
    GetPiecesAt(fromPosition).emplace<EmptyPiece>(fromPosition);
}

bool Square::VerifyIfKingBeingCheck_(const Position &piecePosition, const Color &pieceColor,
                                     const Position &kingPosition) const {
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
        throw PgnException("Cannot find direction");
    }

    auto FindNextNonEmpty = [this](const Position &start, int dr, int dc) {
        assert(dr != 0 || dc != 0);
        Position pos = {start.row + dr, start.col + dc};
        while (pos.IsValid()) {
            if (!IsEmptyAt(pos)) {
                break;
            }
            pos.Shift(dr, dc);
        }
        return pos;
    };

    Position possibleOpponent = FindNextNonEmpty(piecePosition, dr[index], dc[index]);
    Position possibleObstacle = FindNextNonEmpty(piecePosition, -dr[index], -dc[index]);
    bool kingChecked = false;

    if (possibleObstacle.IsValid() && possibleObstacle == kingPosition && possibleOpponent.IsValid()) {
        std::visit(
            [&](const auto &opponent) {
                if (opponent.GetColor() != Color::Undefined && opponent.GetColor() != pieceColor) {
                    kingChecked = opponent.IsValidBasicMove(kingPosition, piecePosition);
                }
            },
            GetPiecesAt(possibleOpponent));
    }

    return kingChecked;
}

void Square::MovePiece(const FromPosition &fromPosition, const ToPosition &toPosition) {
    if (!fromPosition.IsValid() || !toPosition.IsValid()) {
        std::cerr << "[THO][E] Error while moving : Invalid position" << std::endl;
        throw PgnException("Error while moving : Invalid position");
    }
    auto tmpF = fromPosition;
    auto tmpT = toPosition;
    Color movingColor = Color::Undefined;
    PieceType movingType = PieceType::Undefined;

    std::visit(
        [&](const auto &piece) {
            movingColor = piece.GetColor();
            movingType = piece.GetType();
        },
        GetPiecesAt(fromPosition));

    GetPiecesAt(fromPosition).swap(GetPiecesAt(toPosition));
    std::visit([&](auto &&piece) { piece.SetPosition(tmpF); }, GetPiecesAt(fromPosition));
    std::visit([&](auto &&piece) { piece.SetPosition(tmpT); }, GetPiecesAt(toPosition));

    MovePieceInCache_(movingColor, movingType, fromPosition, toPosition);
}

void Square::AttackPiece(const FromPosition &fromPosition, const ToPosition &toPosition) {
    if (!fromPosition.IsValid() || !toPosition.IsValid()) {
        std::cerr << "[THO][E] Error while moving : Invalid position" << std::endl;
        throw PgnException("Error while attacking : Invalid position");
    }
    auto tmpF = fromPosition;
    auto tmpT = toPosition;
    Color attackerColor = Color::Undefined;
    PieceType attackerType = PieceType::Undefined;
    Color capturedColor = Color::Undefined;
    PieceType capturedType = PieceType::Undefined;

    std::visit(
        [&](const auto &piece) {
            attackerColor = piece.GetColor();
            attackerType = piece.GetType();
        },
        GetPiecesAt(fromPosition));

    std::visit(
        [&](const auto &piece) {
            capturedColor = piece.GetColor();
            capturedType = piece.GetType();
        },
        GetPiecesAt(toPosition));

    GetPiecesAt(fromPosition).swap(GetPiecesAt(toPosition));
    std::visit([&](auto &&piece) { piece.SetPosition(tmpT); }, GetPiecesAt(toPosition));
    GetPiecesAt(fromPosition).emplace<EmptyPiece>(tmpF);

    RemovePieceFromCache_(capturedColor, capturedType, toPosition);
    MovePieceInCache_(attackerColor, attackerType, fromPosition, toPosition);

    if (enPassant_.IsValid()) {
        std::visit([&](const auto &piece) { RemovePieceFromCache_(piece.GetColor(), piece.GetType(), enPassant_); },
                   GetPiecesAt(enPassant_));
        GetPiecesAt(enPassant_).emplace<EmptyPiece>(enPassant_);
        enPassant_.Reset();
    }
}

void Square::ValidateMove_(const Position &kingPosition, const Position &piecePosition, MoveData &moveData,
                           bool &isValid) const {
    if (isValid) {
        if (!(!AreOnFileOrRowOrDiagonal(kingPosition, piecePosition) ||
              (AreOnFileOrRowOrDiagonal(kingPosition, piecePosition, moveData.toPosition)))) {
            if (VerifyIfKingBeingCheck_(piecePosition, moveData.color, kingPosition)) {
                isValid = false;
            }
        }
    }
    if (isValid) {
        if ((moveData.fromPosition.row != -1 && moveData.fromPosition.row != piecePosition.row) ||
            (moveData.fromPosition.col != -1 && moveData.fromPosition.col != piecePosition.col)) {
            isValid = false;
        } else {
            moveData.fromPosition = piecePosition;
        }
    }
}

} // namespace pgn