use crate::types::{Color, PieceType, Position};

/// BasePieceData holds the shared data from C++ BasePiece
/// In Rust, we separate data from behavior (traits)
#[derive(Debug, Clone)]
pub struct BasePieceData {
    pub piece_type: PieceType,
    pub color: Color,
    pub position: Position,
}

impl BasePieceData {
    pub fn new(piece_type: PieceType, color: Color, position: Position) -> Self {
        Self {
            piece_type,
            color,
            position,
        }
    }

    pub fn get_draw(&self) -> String {
        format!("{}{}", self.color, self.piece_type)
    }
}

/// The Piece trait replaces C++ virtual functions and CRTP pattern
/// This trait defines the interface that all piece types must implement
/// 
/// KEY RUST CONCEPT:
/// - In C++ you used `this auto &&self` with friend functions for CRTP
/// - In Rust, we use a trait with default methods that call required methods
/// - Concrete types implement the required methods (_impl variants)
pub trait Piece {
    /// Get reference to the shared base data
    fn base(&self) -> &BasePieceData;

    /// Mutable access to base data for moves
    fn base_mut(&mut self) -> &mut BasePieceData;

    // Default implementations that delegate to concrete implementations
    fn is_valid_basic_move(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_basic_move_impl(to_position, validate_king_check)
    }

    fn is_valid_attack_move(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_attack_move_impl(to_position, validate_king_check)
    }

    // Required implementations (like C++ private friend functions)
    fn is_valid_basic_move_impl(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool;

    fn is_valid_attack_move_impl(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool;
}

/// Helper function similar to C++ BasePiece::ValidateMove_<IsEmpty>
/// 
/// KEY RUST CONCEPT:
/// - C++ used template parameter IsEmpty (functor/lambda)
/// - Rust uses generic with trait bound Fn (closure/function pointer)
/// - The `impl Fn(Position) -> bool` means "any type that implements Fn"
pub fn validate_move<F>(
    d_row: i32,
    d_col: i32,
    to_position: Position,
    validate_king_check: Option<Position>,
    from_position: Position,
    is_empty: F,
) -> bool
where
    F: Fn(Position) -> bool,
{
    // Normalize direction (C++ used dRow /= abs(dRow))
    let d_row = if d_row != 0 {
        d_row / d_row.abs()
    } else {
        0
    };
    let d_col = if d_col != 0 {
        d_col / d_col.abs()
    } else {
        0
    };

    assert!(d_row != 0 || d_col != 0, "Move must change position");

    let mut p = from_position;
    p.shift(d_row, d_col);

    while p.is_valid() {
        if p == to_position {
            return true;
        }
        if !is_empty(p) {
            // Allow moving through the king position being checked
            if let Some(king_pos) = validate_king_check {
                if king_pos != p {
                    return false;
                }
            } else {
                return false;
            }
        }
        p.shift(d_row, d_col);
    }
    false
}
