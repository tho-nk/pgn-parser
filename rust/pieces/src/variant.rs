use crate::base_piece::{BasePieceData, Piece as PieceTrait};
use crate::pieces::{Bishop, EmptyPiece, King, Knight, Pawn, Queen, Rook};
use crate::types::Position;

/// PieceVariant enum is the Rust equivalent of C++:
/// using Piece = std::variant<EmptyPiece, Bishop, King, Knight, Pawn, Queen, Rook>;
/// 
/// KEY RUST CONCEPT:
/// - C++ std::variant is a tagged union (type-safe union)
/// - Rust enum IS a tagged union natively
/// - std::visit in C++ becomes `match` in Rust
/// - This gives zero-cost abstraction with static dispatch
/// 
/// IMPORTANT: We implement the Piece trait directly on PieceVariant!
/// This means PieceVariant can be used anywhere that accepts &dyn Piece
/// and we get automatic delegation to the inner type with one match per trait method.
#[derive(Debug, Clone)]
pub enum PieceVariant {
    Empty(EmptyPiece),
    Bishop(Bishop),
    King(King),
    Knight(Knight),
    Pawn(Pawn),
    Queen(Queen),
    Rook(Rook),
}

/// Implement the Piece trait for the enum itself
/// This is the idiomatic Rust way - delegate once in the trait impl
/// instead of pattern matching at every call site!
impl PieceTrait for PieceVariant {
    fn base(&self) -> &BasePieceData {
        match self {
            PieceVariant::Empty(p) => p.base(),
            PieceVariant::Bishop(p) => p.base(),
            PieceVariant::King(p) => p.base(),
            PieceVariant::Knight(p) => p.base(),
            PieceVariant::Pawn(p) => p.base(),
            PieceVariant::Queen(p) => p.base(),
            PieceVariant::Rook(p) => p.base(),
        }
    }

    fn base_mut(&mut self) -> &mut BasePieceData {
        match self {
            PieceVariant::Empty(p) => p.base_mut(),
            PieceVariant::Bishop(p) => p.base_mut(),
            PieceVariant::King(p) => p.base_mut(),
            PieceVariant::Knight(p) => p.base_mut(),
            PieceVariant::Pawn(p) => p.base_mut(),
            PieceVariant::Queen(p) => p.base_mut(),
            PieceVariant::Rook(p) => p.base_mut(),
        }
    }

    fn is_valid_basic_move_impl(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool {
        match self {
            PieceVariant::Empty(p) => p.is_valid_basic_move_impl(to_position, validate_king_check),
            PieceVariant::Bishop(p) => p.is_valid_basic_move_impl(to_position, validate_king_check),
            PieceVariant::King(p) => p.is_valid_basic_move_impl(to_position, validate_king_check),
            PieceVariant::Knight(p) => p.is_valid_basic_move_impl(to_position, validate_king_check),
            PieceVariant::Pawn(p) => p.is_valid_basic_move_impl(to_position, validate_king_check),
            PieceVariant::Queen(p) => p.is_valid_basic_move_impl(to_position, validate_king_check),
            PieceVariant::Rook(p) => p.is_valid_basic_move_impl(to_position, validate_king_check),
        }
    }

    fn is_valid_attack_move_impl(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool {
        match self {
            PieceVariant::Empty(p) => p.is_valid_attack_move_impl(to_position, validate_king_check),
            PieceVariant::Bishop(p) => p.is_valid_attack_move_impl(to_position, validate_king_check),
            PieceVariant::King(p) => p.is_valid_attack_move_impl(to_position, validate_king_check),
            PieceVariant::Knight(p) => p.is_valid_attack_move_impl(to_position, validate_king_check),
            PieceVariant::Pawn(p) => p.is_valid_attack_move_impl(to_position, validate_king_check),
            PieceVariant::Queen(p) => p.is_valid_attack_move_impl(to_position, validate_king_check),
            PieceVariant::Rook(p) => p.is_valid_attack_move_impl(to_position, validate_king_check),
        }
    }
}

impl PieceVariant {
    /// Check if this is an empty square
    /// Equivalent to C++ std::holds_alternative<EmptyPiece>(piece)
    pub fn is_empty(&self) -> bool {
        matches!(self, PieceVariant::Empty(_))
    }
}

/// Board type: 8x8 array of pieces
/// Maps to C++: using Pieces = std::array<std::array<Piece, COLUMNS>, ROWS>;
pub type Board = [[PieceVariant; 8]; 8];

/// Helper to create an empty board
pub fn create_empty_board() -> Board {
    // Rust requires explicit initialization for arrays
    std::array::from_fn(|row| {
        std::array::from_fn(|col| {
            PieceVariant::Empty(EmptyPiece::new(Position::new(row as i32, col as i32)))
        })
    })
}
