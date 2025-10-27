/// Board dimensions
pub const COLUMNS: usize = 8;
pub const ROWS: usize = 8;

/// Color enumeration maps to C++ `enum class Color`
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Color {
    White,
    Black,
    Undefined,
}

impl std::fmt::Display for Color {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Color::White => write!(f, "White"),
            Color::Black => write!(f, "Black"),
            Color::Undefined => write!(f, "Undefined"),
        }
    }
}

/// PieceType enumeration maps to C++ `enum class PieceType`
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

impl std::fmt::Display for PieceType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            PieceType::King => write!(f, "K"),
            PieceType::Queen => write!(f, "Q"),
            PieceType::Rook => write!(f, "R"),
            PieceType::Bishop => write!(f, "B"),
            PieceType::Knight => write!(f, "N"),
            PieceType::Pawn => write!(f, "P"),
            PieceType::Undefined => write!(f, "?"),
        }
    }
}

/// Position struct maps to C++ `struct Position`
/// Note: In Rust we derive Copy for small, trivial types
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Position {
    pub row: i32,
    pub col: i32,
}

impl Position {
    pub fn new(row: i32, col: i32) -> Self {
        Self { row, col }
    }

    pub fn shift(&mut self, dr: i32, dc: i32) {
        self.row += dr;
        self.col += dc;
    }

    pub fn is_valid(&self) -> bool {
        self.row >= 0 && self.row < ROWS as i32 && self.col >= 0 && self.col < COLUMNS as i32
    }

    pub fn reset(&mut self) {
        self.row = -1;
        self.col = -1;
    }
}

impl Default for Position {
    fn default() -> Self {
        Self { row: -1, col: -1 }
    }
}

/// Helper functions for position relationships
pub fn are_on_file(p1: &Position, p2: &Position) -> bool {
    p1.col == p2.col
}

pub fn are_on_row(p1: &Position, p2: &Position) -> bool {
    p1.row == p2.row
}

pub fn are_on_diagonal(p1: &Position, p2: &Position) -> bool {
    (p1.row - p2.row).abs() == (p1.col - p2.col).abs()
}
