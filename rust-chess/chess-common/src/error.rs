//! Error types for chess operations

use thiserror::Error;

#[derive(Error, Debug)]
pub enum ChessError {
    #[error("Invalid move: {0}")]
    InvalidMove(String),
    
    #[error("Parse error: {0}")]
    ParseError(String),
    
    #[error("IO error: {0}")]
    IoError(#[from] std::io::Error),
    
    #[error("Invalid position: row={0}, col={1}")]
    InvalidPosition(i32, i32),
    
    #[error("Piece not found: {0}")]
    PieceNotFound(String),
    
    #[error("Invalid castling: {0}")]
    InvalidCastling(String),
    
    #[error("King in check: {0}")]
    KingInCheck(String),
    
    #[error("File format error: {0}")]
    FileFormatError(String),
}

pub type Result<T> = std::result::Result<T, ChessError>;
