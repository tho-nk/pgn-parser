//! # PGN Parser
//! 
//! This library provides functionality to parse Portable Game Notation (PGN) files,
//! which is the standard format for recording chess games.
//! 
//! ## C++ to Rust Translation
//! 
//! This module ports the C++ `ParsingHelper` class from:
//! - `src/chess/common/include/ParsingHelper.hpp`
//! - `src/chess/common/src/ParsingHelper.cpp`
//! 
//! ### Key Translations
//! 
//! | C++ | Rust |
//! |-----|------|
//! | `std::string_view` | `&str` |
//! | `std::string` | `String` |
//! | `std::queue<std::string>` | `VecDeque<String>` |
//! | `std::ifstream` | `std::fs::File` + `BufReader` |
//! | `std::filesystem::path` | `std::path::Path` |
//! | `size_t` | `usize` |
//! 
//! ## Usage Example
//! 
//! ```rust,no_run
//! use pgn_parser::parse_file;
//! use std::path::Path;
//! 
//! let result = parse_file(Path::new("game.pgn")).unwrap();
//! 
//! for round in result.round_queue {
//!     println!("Round: {}", round);
//! }
//! println!("Last: {}", result.last_run);
//! ```

pub mod parsing;

// Re-export commonly used items
pub use parsing::{
    is_balanced, get_comment, get_next_non_space, get_next_space,
    left_trim, right_trim, trim_space, 
    remove_unwanted_chars, remove_3_dot,
    process_game_comment, parse_file, parse_text,
    ParsingResult,
};
