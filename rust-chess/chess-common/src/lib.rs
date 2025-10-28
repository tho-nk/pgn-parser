//! # Chess Common Library
//!
//! Core types, utilities, and PGN parsing for chess applications.
//!
//! This module provides:
//! - Common chess types (Color, Position, PieceType, MoveType)
//! - Error handling
//! - PGN file parsing

pub mod types;
pub mod error;
pub mod parser;

// Re-export commonly used items
pub use types::{Color, Position, PieceType, MoveType, MoveData};
pub use error::{ChessError, Result};
pub use parser::{parse_file, parse_text, ParsingResult};
