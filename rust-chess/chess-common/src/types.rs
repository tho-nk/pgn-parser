//! Core chess types
//!
//! Maps from C++ src/chess/common/include/Types.hpp

use std::fmt;

/// Chess piece color
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum Color {
    White,
    Black,
}

impl fmt::Display for Color {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Color::White => write!(f, "White"),
            Color::Black => write!(f, "Black"),
        }
    }
}

/// Position on chess board (0-7 for both row and col)
/// Row 0 = white's back rank, Row 7 = black's back rank
/// Col 0 = a-file, Col 7 = h-file
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct Position {
    pub row: i32,
    pub col: i32,
}

impl Position {
    pub fn new(row: i32, col: i32) -> Self {
        Self { row, col }
    }
    
    /// Check if position is on the board
    pub fn is_valid(&self) -> bool {
        self.row >= 0 && self.row < 8 && self.col >= 0 && self.col < 8
    }
    
    /// Reset to invalid position
    pub fn reset(&mut self) {
        self.row = -1;
        self.col = -1;
    }
    
    /// Shift position by delta
    pub fn shift(&mut self, dr: i32, dc: i32) {
        self.row += dr;
        self.col += dc;
    }
    
    /// Convert to algebraic notation (e.g., "e4")
    pub fn to_algebraic(&self) -> String {
        if !self.is_valid() {
            return "??".to_string();
        }
        let file = (b'a' + self.col as u8) as char;
        let rank = (self.row + 1).to_string();
        format!("{}{}", file, rank)
    }
    
    /// Parse from algebraic notation (e.g., "e4" -> Position{3, 4})
    pub fn from_algebraic(s: &str) -> Option<Self> {
        if s.len() < 2 {
            return None;
        }
        let mut chars = s.chars();
        let file = chars.next()?;
        let rank = chars.next()?;
        
        let col = (file as u8).checked_sub(b'a')? as i32;
        let row = rank.to_digit(10)? as i32 - 1;
        
        let pos = Position::new(row, col);
        if pos.is_valid() {
            Some(pos)
        } else {
            None
        }
    }
}

impl Default for Position {
    fn default() -> Self {
        Self { row: -1, col: -1 }
    }
}

impl fmt::Display for Position {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.to_algebraic())
    }
}

/// Chess piece types
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn,
}

impl fmt::Display for PieceType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            PieceType::King => write!(f, "K"),
            PieceType::Queen => write!(f, "Q"),
            PieceType::Rook => write!(f, "R"),
            PieceType::Bishop => write!(f, "B"),
            PieceType::Knight => write!(f, "N"),
            PieceType::Pawn => write!(f, "P"),
        }
    }
}

/// Move types
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum MoveType {
    Basic,
    Attack,
    Promotion,
    Castling,
    AttackPromotion,
}

/// Data structure holding information about a move
#[derive(Debug, Clone)]
pub struct MoveData {
    pub piece_type: PieceType,
    pub color: Color,
    pub from_position: Position,
    pub to_position: Position,
    pub move_type: MoveType,
    pub promotion_type: Option<PieceType>,
    pub is_kingside_castling: bool,
    pub is_queenside_castling: bool,
}

impl MoveData {
    pub fn new(color: Color) -> Self {
        Self {
            piece_type: PieceType::Pawn,
            color,
            from_position: Position::default(),
            to_position: Position::default(),
            move_type: MoveType::Basic,
            promotion_type: None,
            is_kingside_castling: false,
            is_queenside_castling: false,
        }
    }
}

// Helper functions for position relationships

/// Check if positions are on the same file (column)
pub fn are_on_file(a: Position, b: Position) -> bool {
    a.col == b.col
}

/// Check if positions are on the same row (rank)
pub fn are_on_row(a: Position, b: Position) -> bool {
    a.row == b.row
}

/// Check if positions are on the same diagonal
pub fn are_on_diagonal(a: Position, b: Position) -> bool {
    (a.row - b.row).abs() == (a.col - b.col).abs()
}

/// Check if positions are on file, row, or diagonal
pub fn are_on_file_or_row_or_diagonal(a: Position, b: Position, c: Option<Position>) -> bool {
    if let Some(c_pos) = c {
        // Check if all three are aligned
        (are_on_file(a, b) && are_on_file(b, c_pos))
            || (are_on_row(a, b) && are_on_row(b, c_pos))
            || (are_on_diagonal(a, b) && are_on_diagonal(b, c_pos))
    } else {
        are_on_file(a, b) || are_on_row(a, b) || are_on_diagonal(a, b)
    }
}

/// Convert string to piece type
pub fn string_to_piece_type(s: &str) -> Option<PieceType> {
    match s {
        "K" => Some(PieceType::King),
        "Q" => Some(PieceType::Queen),
        "R" => Some(PieceType::Rook),
        "B" => Some(PieceType::Bishop),
        "N" => Some(PieceType::Knight),
        "" => Some(PieceType::Pawn),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_position_validity() {
        assert!(Position::new(0, 0).is_valid());
        assert!(Position::new(7, 7).is_valid());
        assert!(!Position::new(-1, 0).is_valid());
        assert!(!Position::new(0, 8).is_valid());
    }

    #[test]
    fn test_algebraic_notation() {
        let pos = Position::new(0, 0);
        assert_eq!(pos.to_algebraic(), "a1");
        
        let pos = Position::new(3, 4);
        assert_eq!(pos.to_algebraic(), "e4");
        
        let pos = Position::from_algebraic("e4").unwrap();
        assert_eq!(pos, Position::new(3, 4));
    }

    #[test]
    fn test_position_relationships() {
        let a1 = Position::new(0, 0);
        let a8 = Position::new(7, 0);
        let h1 = Position::new(0, 7);
        let h8 = Position::new(7, 7);
        
        assert!(are_on_file(a1, a8));
        assert!(are_on_row(a1, h1));
        assert!(are_on_diagonal(a1, h8));
    }
}
