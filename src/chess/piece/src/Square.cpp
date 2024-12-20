#include "piece/include/Square.hpp"
#include "common/include/ParsingHelper.hpp"
#include "common/include/PgnException.hpp"
#include "move/include/Round.hpp"
#include <cassert>
#include <ranges>

namespace pgn {

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
}

void Square::Run() {
    for (const auto &round : rounds_) {
        round.Run();
    }
}

void Square::LoadPGN(const std::filesystem::path &filePath) {
    ResetState_();
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

PiecesReference Square::GetPieceOfTypeAndColor_(const PieceType &pieceType, const Color &color,
                                                const FromPosition &fromPosition) const noexcept {
    PiecesReference subPieces;
    if (fromPosition.IsValid()) {
        subPieces.push_back(std::cref(GetPiecesAt(fromPosition)));
    } else {
        for (const auto &file : GetPieces()) {
            for (const auto &var : file) {
                std::visit(
                    [&](const auto &value) {
                        if (value.GetType() == pieceType && value.GetColor() == color) {
                            subPieces.push_back(std::cref(GetPiecesAt(value.GetPosition())));
                        }
                    },
                    var);
            }
        }
    }
    return subPieces;
}

Position Square::GetKingPosition_(const Color &color) const {
    const auto &kings = GetPieceOfTypeAndColor_(PieceType::King, color, Position{});
    return std::get<King>(kings.at(0).get()).GetPosition();
}

void Square::ProcessBasicMove(MoveData &moveData) const {
    if (moveData.fromPosition.IsValid()) {
        return;
    }
    bool isValid = false;
    const auto subPieces = GetPieceOfTypeAndColor_(moveData.pieceType, moveData.color, moveData.fromPosition);

    const auto &kingPosition = GetKingPosition_(moveData.color);
    for (const auto &it : subPieces) {
        std::visit(
            [&](const auto &piece) {
                isValid = piece.IsValidBasicMove(moveData.toPosition);
                ValidateMove_(kingPosition, piece.GetPosition(), moveData, isValid);
            },
            it.get());
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
    const auto subPieces = GetPieceOfTypeAndColor_(moveData.pieceType, moveData.color, moveData.fromPosition);
    const auto &kingPosition = GetKingPosition_(moveData.color);
    for (const auto &it : subPieces) {
        std::visit(
            [&](const auto &piece) {
                isValid = piece.IsValidAttackMove(moveData.toPosition);
                ValidateMove_(kingPosition, piece.GetPosition(), moveData, isValid);
            },
            it.get());
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
        break;
    }
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

    GetPiecesAt(fromPosition).swap(GetPiecesAt(toPosition));
    std::visit([&](auto &&piece) { piece.SetPosition(tmpF); }, GetPiecesAt(fromPosition));
    std::visit([&](auto &&piece) { piece.SetPosition(tmpT); }, GetPiecesAt(toPosition));
}

void Square::AttackPiece(const FromPosition &fromPosition, const ToPosition &toPosition) {
    if (!fromPosition.IsValid() || !toPosition.IsValid()) {
        std::cerr << "[THO][E] Error while moving : Invalid position" << std::endl;
        throw PgnException("Error while attacking : Invalid position");
    }
    auto tmpF = fromPosition;
    auto tmpT = toPosition;

    GetPiecesAt(fromPosition).swap(GetPiecesAt(toPosition));
    std::visit([&](auto &&piece) { piece.SetPosition(tmpT); }, GetPiecesAt(toPosition));
    GetPiecesAt(fromPosition).emplace<EmptyPiece>(tmpF);

    if (enPassant_.IsValid()) {
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