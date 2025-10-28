//! Re-export types from chess-common for backwards compatibility
//!
//! This module now just re-exports types from chess-common to avoid duplication.

pub use chess_common::{Color, Position, PieceType};
pub use chess_common::types::{are_on_file, are_on_row, are_on_diagonal};

/// Board dimensions
pub const COLUMNS: usize = 8;
pub const ROWS: usize = 8;

