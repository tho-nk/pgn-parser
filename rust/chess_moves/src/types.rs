/// Re-export types from pieces crate or define them here
/// For this standalone module, we'll define the minimal types needed

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Color {
    White,
    Black,
    Undefined,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn,
    Undefined,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum MoveType {
    BasicMove,
    AttackMove,
    PromotionMove,
    CastlingMove,
    AttackPromotionMove,
    Undefined,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Position {
    pub row: i32,
    pub col: i32,
}

impl Position {
    pub fn new(row: i32, col: i32) -> Self {
        Self { row, col }
    }

    pub fn invalid() -> Self {
        Self { row: -1, col: -1 }
    }

    pub fn is_valid(&self) -> bool {
        self.row >= 0 && self.row < 8 && self.col >= 0 && self.col < 8
    }
}

impl Default for Position {
    fn default() -> Self {
        Self::invalid()
    }
}

/// MoveData holds information about a move
/// Maps to C++ struct MoveData
#[derive(Debug, Clone)]
pub struct MoveData {
    pub color: Color,
    pub from_position: Position,
    pub to_position: Position,
    pub piece_type: PieceType,
    pub promotion_type: PieceType,
    // Castling-specific fields
    pub king_source: Position,
    pub king_destination: Position,
    pub rook_source: Position,
    pub rook_destination: Position,
}

impl MoveData {
    pub fn new(color: Color) -> Self {
        Self {
            color,
            from_position: Position::invalid(),
            to_position: Position::invalid(),
            piece_type: PieceType::Undefined,
            promotion_type: PieceType::Undefined,
            king_source: Position::invalid(),
            king_destination: Position::invalid(),
            rook_source: Position::invalid(),
            rook_destination: Position::invalid(),
        }
    }
}

/// Helper to parse piece type from string
pub fn string_to_piece_type(s: &str) -> PieceType {
    match s {
        "K" => PieceType::King,
        "Q" => PieceType::Queen,
        "R" => PieceType::Rook,
        "B" => PieceType::Bishop,
        "N" => PieceType::Knight,
        "P" | "" => PieceType::Pawn,
        _ => PieceType::Undefined,
    }
}
