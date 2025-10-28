//! Board state management
//!
//! This module implements the chess board (C++ Square class).
//! It manages the 8x8 board state, piece positions, and move execution.

use chess_common::{Color, Position, PieceType, MoveData, Result, ChessError};
use chess_common::types::{are_on_file_or_row_or_diagonal};
use crate::variant::PieceVariant;
use crate::base_piece::Piece;
use crate::pieces::*;

/// Chess board managing the game state
/// Maps to C++ Square class
pub struct Board {
    /// 8x8 board of pieces
    /// Row 0 = white's back rank (a1-h1)
    /// Row 7 = black's back rank (a8-h8)
    pieces: [[PieceVariant; 8]; 8],
    
    /// En passant target square
    en_passant: Option<Position>,
}

impl Board {
    /// Create a new board with standard starting position
    pub fn new() -> Self {
        let mut board = Self {
            pieces: Self::create_empty_board(),
            en_passant: None,
        };
        board.reset_to_starting_position();
        board
    }
    
    /// Create an empty board (all empty squares)
    fn create_empty_board() -> [[PieceVariant; 8]; 8] {
        std::array::from_fn(|row| {
            std::array::from_fn(|col| {
                PieceVariant::Empty(EmptyPiece::new(Position::new(row as i32, col as i32)))
            })
        })
    }
    
    /// Reset board to standard chess starting position
    /// Maps to C++ Square::ResetState_()
    pub fn reset_to_starting_position(&mut self) {
        // Clear middle ranks
        for row in 2..6 {
            for col in 0..8 {
                self.pieces[row][col] = PieceVariant::Empty(
                    EmptyPiece::new(Position::new(row as i32, col as i32))
                );
            }
        }
        
        // Pawns
        for col in 0..8 {
            self.pieces[1][col] = PieceVariant::Pawn(
                Pawn::new(Color::White, Position::new(1, col as i32))
            );
            self.pieces[6][col] = PieceVariant::Pawn(
                Pawn::new(Color::Black, Position::new(6, col as i32))
            );
        }
        
        // Rooks
        self.pieces[0][0] = PieceVariant::Rook(Rook::new(Color::White, Position::new(0, 0)));
        self.pieces[0][7] = PieceVariant::Rook(Rook::new(Color::White, Position::new(0, 7)));
        self.pieces[7][0] = PieceVariant::Rook(Rook::new(Color::Black, Position::new(7, 0)));
        self.pieces[7][7] = PieceVariant::Rook(Rook::new(Color::Black, Position::new(7, 7)));
        
        // Knights
        self.pieces[0][1] = PieceVariant::Knight(Knight::new(Color::White, Position::new(0, 1)));
        self.pieces[0][6] = PieceVariant::Knight(Knight::new(Color::White, Position::new(0, 6)));
        self.pieces[7][1] = PieceVariant::Knight(Knight::new(Color::Black, Position::new(7, 1)));
        self.pieces[7][6] = PieceVariant::Knight(Knight::new(Color::Black, Position::new(7, 6)));
        
        // Bishops
        self.pieces[0][2] = PieceVariant::Bishop(Bishop::new(Color::White, Position::new(0, 2)));
        self.pieces[0][5] = PieceVariant::Bishop(Bishop::new(Color::White, Position::new(0, 5)));
        self.pieces[7][2] = PieceVariant::Bishop(Bishop::new(Color::Black, Position::new(7, 2)));
        self.pieces[7][5] = PieceVariant::Bishop(Bishop::new(Color::Black, Position::new(7, 5)));
        
        // Queens
        self.pieces[0][3] = PieceVariant::Queen(Queen::new(Color::White, Position::new(0, 3)));
        self.pieces[7][3] = PieceVariant::Queen(Queen::new(Color::Black, Position::new(7, 3)));
        
        // Kings
        self.pieces[0][4] = PieceVariant::King(King::new(Color::White, Position::new(0, 4)));
        self.pieces[7][4] = PieceVariant::King(King::new(Color::Black, Position::new(7, 4)));
        
        self.en_passant = None;
    }
    
    /// Get piece at position (immutable)
    pub fn get_piece(&self, pos: Position) -> Result<&PieceVariant> {
        if !pos.is_valid() {
            return Err(ChessError::InvalidPosition(pos.row, pos.col));
        }
        Ok(&self.pieces[pos.row as usize][pos.col as usize])
    }
    
    /// Get piece at position (mutable)
    pub fn get_piece_mut(&mut self, pos: Position) -> Result<&mut PieceVariant> {
        if !pos.is_valid() {
            return Err(ChessError::InvalidPosition(pos.row, pos.col));
        }
        Ok(&mut self.pieces[pos.row as usize][pos.col as usize])
    }
    
    /// Check if square is empty
    pub fn is_empty(&self, pos: Position) -> bool {
        self.get_piece(pos).map(|p| p.is_empty()).unwrap_or(false)
    }
    
    /// Set en passant target square
    pub fn set_en_passant(&mut self, pos: Option<Position>) {
        self.en_passant = pos;
    }
    
    /// Get en passant target square
    pub fn get_en_passant(&self) -> Option<Position> {
        self.en_passant
    }
    
    /// Find king position for given color
    /// Maps to C++ Square::GetKingPosition_()
    pub fn get_king_position(&self, color: Color) -> Result<Position> {
        for row in 0..8 {
            for col in 0..8 {
                if let PieceVariant::King(king) = &self.pieces[row][col] {
                    if king.base().color == color {
                        return Ok(king.base().position);
                    }
                }
            }
        }
        Err(ChessError::PieceNotFound(format!("{} King not found", color)))
    }
    
    /// Find all pieces of given type and color
    /// Optionally filtered by from_position hint
    /// Maps to C++ Square::GetPieceOfTypeAndColor_()
    pub fn get_pieces_of_type_and_color(
        &self,
        piece_type: PieceType,
        color: Color,
        from_position: Option<Position>,
    ) -> Vec<Position> {
        let mut positions = Vec::new();
        
        if let Some(pos) = from_position {
            if pos.is_valid() {
                positions.push(pos);
                return positions;
            }
        }
        
        for row in 0..8 {
            for col in 0..8 {
                let pos = Position::new(row, col);
                if let Ok(piece) = self.get_piece(pos) {
                    let base = piece.base();
                    if base.piece_type == piece_type && base.color == color {
                        positions.push(pos);
                    }
                }
            }
        }
        
        positions
    }
    
    /// Process basic move - find valid piece and update move data
    /// Maps to C++ Square::ProcessBasicMove()
    pub fn process_basic_move(&self, move_data: &mut MoveData) -> Result<()> {
        if move_data.from_position.is_valid() {
            return Ok(()); // Already has from position
        }
        
        let candidates = self.get_pieces_of_type_and_color(
            move_data.piece_type,
            move_data.color,
            Some(move_data.from_position),
        );
        
        let king_pos = self.get_king_position(move_data.color)?;
        
        for &piece_pos in &candidates {
            let piece = self.get_piece(piece_pos)?;
            
            // Check if piece can make this move
            let is_valid = piece.is_valid_basic_move(move_data.to_position, self.en_passant);
            
            if is_valid {
                // Validate that this move doesn't expose king
                if self.validate_move_safety(king_pos, piece_pos, move_data)? {
                    move_data.from_position = piece_pos;
                    return Ok(());
                }
            }
        }
        
        Err(ChessError::InvalidMove(format!(
            "No valid {} for basic move to {}",
            move_data.piece_type,
            move_data.to_position
        )))
    }
    
    /// Process attack move - find valid piece and update move data
    /// Maps to C++ Square::ProcessAttackMove()
    pub fn process_attack_move(&self, move_data: &mut MoveData) -> Result<()> {
        if move_data.from_position.is_valid() {
            return Ok(());
        }
        
        let candidates = self.get_pieces_of_type_and_color(
            move_data.piece_type,
            move_data.color,
            Some(move_data.from_position),
        );
        
        let king_pos = self.get_king_position(move_data.color)?;
        
        for &piece_pos in &candidates {
            let piece = self.get_piece(piece_pos)?;
            
            // Check if piece can attack this square
            let is_valid = piece.is_valid_attack_move(move_data.to_position, self.en_passant);
            
            if is_valid {
                if self.validate_move_safety(king_pos, piece_pos, move_data)? {
                    move_data.from_position = piece_pos;
                    return Ok(());
                }
            }
        }
        
        Err(ChessError::InvalidMove(format!(
            "No valid {} for attack move to {}",
            move_data.piece_type,
            move_data.to_position
        )))
    }
    
    /// Validate that a move doesn't expose king to check
    /// Maps to C++ Square::ValidateMove_()
    fn validate_move_safety(
        &self,
        king_pos: Position,
        piece_pos: Position,
        move_data: &MoveData,
    ) -> Result<bool> {
        // Check if piece is pinned to king
        if !are_on_file_or_row_or_diagonal(king_pos, piece_pos, Some(move_data.to_position)) {
            // Piece not aligned with king, check if removing it exposes king
            if are_on_file_or_row_or_diagonal(king_pos, piece_pos, None) {
                if self.verify_king_check_after_move(piece_pos, move_data.color, king_pos)? {
                    return Ok(false); // Move would expose king
                }
            }
        }
        
        // Check from_position hints (file/rank disambiguation)
        if move_data.from_position.row != -1 && move_data.from_position.row != piece_pos.row {
            return Ok(false);
        }
        if move_data.from_position.col != -1 && move_data.from_position.col != piece_pos.col {
            return Ok(false);
        }
        
        Ok(true)
    }
    
    /// Check if king would be in check after piece moves
    /// Maps to C++ Square::VerifyIfKingBeingCheck_()
    fn verify_king_check_after_move(
        &self,
        piece_pos: Position,
        piece_color: Color,
        king_pos: Position,
    ) -> Result<bool> {
        // Direction vectors (8 directions)
        const DR: [i32; 8] = [-1, 0, 1, 1, 1, 0, -1, -1];
        const DC: [i32; 8] = [-1, -1, -1, 0, 1, 1, 1, 0];
        
        // Calculate direction from king to piece
        let mut drow = piece_pos.row - king_pos.row;
        let mut dcol = piece_pos.col - king_pos.col;
        
        // Normalize to -1, 0, or 1
        if drow != 0 {
            drow /= drow.abs();
        }
        if dcol != 0 {
            dcol /= dcol.abs();
        }
        
        // Find direction index
        let dir_idx = DR.iter()
            .zip(DC.iter())
            .position(|(&dr, &dc)| dr == drow && dc == dcol)
            .ok_or_else(|| ChessError::InvalidMove("Cannot find direction".to_string()))?;
        
        // Look for enemy piece that could attack king through this square
        let opponent_pos = self.find_next_non_empty(piece_pos, DR[dir_idx], DC[dir_idx]);
        let obstacle_pos = self.find_next_non_empty(piece_pos, -DR[dir_idx], -DC[dir_idx]);
        
        // Check if king is the obstacle and there's an attacking opponent
        if let Some(obstacle) = obstacle_pos {
            if obstacle == king_pos {
                if let Some(opp_pos) = opponent_pos {
                    let opponent = self.get_piece(opp_pos)?;
                    let opp_base = opponent.base();
                    
                    if opp_base.color != piece_color {
                        // Check if opponent can attack king from its position
                        return Ok(opponent.is_valid_basic_move(king_pos, Some(piece_pos)));
                    }
                }
            }
        }
        
        Ok(false)
    }
    
    /// Find next non-empty square in given direction
    fn find_next_non_empty(&self, start: Position, dr: i32, dc: i32) -> Option<Position> {
        let mut pos = Position::new(start.row + dr, start.col + dc);
        
        while pos.is_valid() {
            if !self.is_empty(pos) {
                return Some(pos);
            }
            pos.shift(dr, dc);
        }
        
        None
    }
    
    /// Move piece from one square to another
    /// Maps to C++ Square::MovePiece()
    pub fn move_piece(&mut self, from: Position, to: Position) -> Result<()> {
        if !from.is_valid() || !to.is_valid() {
            return Err(ChessError::InvalidPosition(from.row, to.col));
        }
        
        let from_idx = (from.row as usize, from.col as usize);
        let to_idx = (to.row as usize, to.col as usize);
        
        // Use a temporary to avoid double mut borrow
        let mut temp = PieceVariant::Empty(EmptyPiece::new(from));
        std::mem::swap(&mut temp, &mut self.pieces[from_idx.0][from_idx.1]);
        std::mem::swap(&mut temp, &mut self.pieces[to_idx.0][to_idx.1]);
        std::mem::swap(&mut temp, &mut self.pieces[from_idx.0][from_idx.1]);
        
        // Update positions
        self.pieces[from_idx.0][from_idx.1].set_position(from);
        self.pieces[to_idx.0][to_idx.1].set_position(to);
        
        Ok(())
    }
    
    /// Attack piece (capture)
    /// Maps to C++ Square::AttackPiece()
    pub fn attack_piece(&mut self, from: Position, to: Position) -> Result<()> {
        if !from.is_valid() || !to.is_valid() {
            return Err(ChessError::InvalidPosition(from.row, to.col));
        }
        
        let from_idx = (from.row as usize, from.col as usize);
        let to_idx = (to.row as usize, to.col as usize);
        
        // Move attacking piece to target (using temp to avoid double borrow)
        let mut temp = PieceVariant::Empty(EmptyPiece::new(from));
        std::mem::swap(&mut temp, &mut self.pieces[from_idx.0][from_idx.1]);
        self.pieces[to_idx.0][to_idx.1] = temp;
        
        // Update position of moved piece
        self.pieces[to_idx.0][to_idx.1].set_position(to);
        
        // Leave empty square at from position
        self.pieces[from_idx.0][from_idx.1] = PieceVariant::Empty(EmptyPiece::new(from));
        
        // Handle en passant capture
        if let Some(ep_pos) = self.en_passant {
            if ep_pos.is_valid() {
                let ep_idx = (ep_pos.row as usize, ep_pos.col as usize);
                self.pieces[ep_idx.0][ep_idx.1] = PieceVariant::Empty(EmptyPiece::new(ep_pos));
                self.en_passant = None;
            }
        }
        
        Ok(())
    }
    
    /// Process pawn promotion
    /// Maps to C++ Square::ProcessPromotionMove()
    pub fn process_promotion(
        &mut self,
        promotion_type: PieceType,
        color: Color,
        from: Position,
        to: Position,
    ) -> Result<()> {
        let to_idx = (to.row as usize, to.col as usize);
        let from_idx = (from.row as usize, from.col as usize);
        
        // Place promoted piece
        self.pieces[to_idx.0][to_idx.1] = match promotion_type {
            PieceType::Queen => PieceVariant::Queen(Queen::new(color, to)),
            PieceType::Rook => PieceVariant::Rook(Rook::new(color, to)),
            PieceType::Bishop => PieceVariant::Bishop(Bishop::new(color, to)),
            PieceType::Knight => PieceVariant::Knight(Knight::new(color, to)),
            _ => return Err(ChessError::InvalidMove(format!(
                "Invalid promotion type: {:?}", promotion_type
            ))),
        };
        
        // Leave empty square
        self.pieces[from_idx.0][from_idx.1] = PieceVariant::Empty(EmptyPiece::new(from));
        
        Ok(())
    }
    
    /// Display board state
    /// Maps to C++ Square::GetCurrentState()
    pub fn to_string(&self) -> String {
        let mut result = String::new();
        
        // Print from rank 8 down to rank 1 (from row 7 to row 0)
        for row in (0..8).rev() {
            for col in 0..8 {
                let piece = &self.pieces[row][col];
                result.push_str(&piece.get_draw());
                if col < 7 {
                    result.push('|');
                }
            }
            result.push('\n');
        }
        
        result
    }
}

impl Default for Board {
    fn default() -> Self {
        Self::new()
    }
}

impl std::fmt::Display for Board {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.to_string())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_board_creation() {
        let board = Board::new();
        
        // Check white pieces
        assert!(!board.is_empty(Position::new(0, 0))); // White rook
        assert!(!board.is_empty(Position::new(0, 4))); // White king
        
        // Check empty squares
        assert!(board.is_empty(Position::new(3, 3)));
        assert!(board.is_empty(Position::new(4, 4)));
    }

    #[test]
    fn test_get_king_position() {
        let board = Board::new();
        
        let white_king = board.get_king_position(Color::White).unwrap();
        assert_eq!(white_king, Position::new(0, 4));
        
        let black_king = board.get_king_position(Color::Black).unwrap();
        assert_eq!(black_king, Position::new(7, 4));
    }

    #[test]
    fn test_move_piece() {
        let mut board = Board::new();
        
        // Move white pawn e2 to e4
        let from = Position::new(1, 4);
        let to = Position::new(3, 4);
        
        board.move_piece(from, to).unwrap();
        
        assert!(board.is_empty(from));
        assert!(!board.is_empty(to));
    }

    #[test]
    fn test_find_pieces() {
        let board = Board::new();
        
        // Find white rooks
        let rooks = board.get_pieces_of_type_and_color(
            PieceType::Rook,
            Color::White,
            None,
        );
        
        assert_eq!(rooks.len(), 2);
        assert!(rooks.contains(&Position::new(0, 0)));
        assert!(rooks.contains(&Position::new(0, 7)));
    }
}
