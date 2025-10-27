// Module declarations
pub mod base_piece;
pub mod pieces;
pub mod types;
pub mod variant;

// Re-export commonly used types for convenience
pub use base_piece::{BasePieceData, Piece};
pub use pieces::{Bishop, EmptyPiece, King, Knight, Pawn, Queen, Rook};
pub use types::{Color, PieceType, Position, COLUMNS, ROWS};
pub use variant::{create_empty_board, Board, PieceVariant};

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_rook_vertical_move() {
        let rook = Rook::new(Color::White, Position::new(0, 0));
        // Rook at (0,0) should be able to move to (5,0) vertically
        assert!(rook.is_valid_basic_move(Position::new(5, 0), None));
    }

    #[test]
    fn test_rook_horizontal_move() {
        let rook = Rook::new(Color::White, Position::new(0, 0));
        // Rook at (0,0) should be able to move to (0,5) horizontally
        assert!(rook.is_valid_basic_move(Position::new(0, 5), None));
    }

    #[test]
    fn test_rook_invalid_diagonal() {
        let rook = Rook::new(Color::White, Position::new(0, 0));
        // Rook cannot move diagonally
        assert!(!rook.is_valid_basic_move(Position::new(3, 3), None));
    }

    #[test]
    fn test_knight_l_move() {
        let knight = Knight::new(Color::White, Position::new(4, 4));
        // Knight at (4,4) can move in L-shape: 2 up, 1 right = (2,5)
        assert!(knight.is_valid_basic_move(Position::new(2, 5), None));
        // Also 2 right, 1 up = (3,6)
        assert!(knight.is_valid_basic_move(Position::new(3, 6), None));
    }

    #[test]
    fn test_knight_invalid_straight() {
        let knight = Knight::new(Color::White, Position::new(4, 4));
        // Knight cannot move straight
        assert!(!knight.is_valid_basic_move(Position::new(4, 6), None));
    }

    #[test]
    fn test_bishop_diagonal_move() {
        let bishop = Bishop::new(Color::Black, Position::new(3, 3));
        // Bishop at (3,3) can move diagonally to (6,6)
        assert!(bishop.is_valid_basic_move(Position::new(6, 6), None));
        // Also to (0,0)
        assert!(bishop.is_valid_basic_move(Position::new(0, 0), None));
    }

    #[test]
    fn test_bishop_invalid_straight() {
        let bishop = Bishop::new(Color::Black, Position::new(3, 3));
        // Bishop cannot move straight
        assert!(!bishop.is_valid_basic_move(Position::new(3, 6), None));
    }

    #[test]
    fn test_empty_piece_no_moves() {
        let empty = EmptyPiece::new(Position::new(2, 2));
        // Empty piece cannot move
        assert!(!empty.is_valid_basic_move(Position::new(3, 3), None));
        assert!(!empty.is_valid_attack_move(Position::new(3, 3), None));
    }

    #[test]
    fn test_variant_static_dispatch() {
        // KEY DIFFERENCE: Now PieceVariant implements Piece trait directly!
        // We can call trait methods directly without extra pattern matching
        let rook_variant = PieceVariant::Rook(Rook::new(Color::White, Position::new(0, 0)));
        
        // Call the trait method directly - cleaner than old approach!
        assert!(rook_variant.is_valid_basic_move(Position::new(5, 0), None));

        let knight_variant = PieceVariant::Knight(Knight::new(Color::Black, Position::new(4, 4)));
        assert!(knight_variant.is_valid_basic_move(Position::new(2, 5), None));
    }

    #[test]
    fn test_variant_trait_object_dispatch() {
        // PieceVariant can be used as &dyn Piece directly now
        let rook_variant = PieceVariant::Rook(Rook::new(Color::White, Position::new(0, 0)));
        
        // Cast to trait object - works because PieceVariant implements Piece
        let piece_ref: &dyn Piece = &rook_variant;
        assert!(piece_ref.is_valid_basic_move(Position::new(5, 0), None));
        
        // Can also access base data through the trait
        assert_eq!(piece_ref.base().piece_type, PieceType::Rook);
    }

    #[test]
    fn test_polymorphic_collection() {
        // Now we can use Vec<PieceVariant> and call trait methods directly!
        let pieces: Vec<PieceVariant> = vec![
            PieceVariant::Rook(Rook::new(Color::White, Position::new(0, 0))),
            PieceVariant::Knight(Knight::new(Color::Black, Position::new(4, 4))),
            PieceVariant::Bishop(Bishop::new(Color::White, Position::new(2, 2))),
        ];
        
        // Iterate and call trait methods - no need for as_piece() wrapper!
        for piece in &pieces {
            let _ = piece.base(); // Access base data
            let _ = piece.is_valid_basic_move(Position::new(5, 5), None);
        }
    }

    #[test]
    fn test_board_creation() {
        let board = create_empty_board();
        // Check that all squares are empty
        for row in 0..ROWS {
            for col in 0..COLUMNS {
                assert!(board[row][col].is_empty());
            }
        }
    }

    #[test]
    fn test_position_helpers() {
        let p1 = Position::new(3, 3);
        let p2 = Position::new(5, 5);
        let p3 = Position::new(3, 7);

        // Test diagonal
        assert!(types::are_on_diagonal(&p1, &p2));
        // Test row
        assert!(types::are_on_row(&p1, &p3));
        // Test file/column
        assert!(types::are_on_file(&p1, &Position::new(7, 3)));
    }

    #[test]
    fn test_queen_horizontal_move() {
        let queen = Queen::new(Color::White, Position::new(3, 3));
        // Queen can move horizontally like a rook
        assert!(queen.is_valid_basic_move(Position::new(3, 7), None));
    }

    #[test]
    fn test_queen_vertical_move() {
        let queen = Queen::new(Color::White, Position::new(3, 3));
        // Queen can move vertically like a rook
        assert!(queen.is_valid_basic_move(Position::new(7, 3), None));
    }

    #[test]
    fn test_queen_diagonal_move() {
        let queen = Queen::new(Color::White, Position::new(3, 3));
        // Queen can move diagonally like a bishop
        assert!(queen.is_valid_basic_move(Position::new(6, 6), None));
        assert!(queen.is_valid_basic_move(Position::new(0, 0), None));
    }

    #[test]
    fn test_queen_invalid_knight_move() {
        let queen = Queen::new(Color::White, Position::new(3, 3));
        // Queen cannot move like a knight
        assert!(!queen.is_valid_basic_move(Position::new(5, 4), None));
    }

    #[test]
    fn test_king_one_square() {
        let king = King::new(Color::White, Position::new(4, 4));
        // King can move one square in any direction
        assert!(king.is_valid_basic_move(Position::new(5, 4), None)); // down
        assert!(king.is_valid_basic_move(Position::new(3, 4), None)); // up
        assert!(king.is_valid_basic_move(Position::new(4, 5), None)); // right
        assert!(king.is_valid_basic_move(Position::new(4, 3), None)); // left
        assert!(king.is_valid_basic_move(Position::new(5, 5), None)); // diagonal
    }

    #[test]
    fn test_king_invalid_two_squares() {
        let king = King::new(Color::White, Position::new(4, 4));
        // King cannot move two squares
        assert!(!king.is_valid_basic_move(Position::new(6, 4), None));
        assert!(!king.is_valid_basic_move(Position::new(2, 4), None));
    }

    #[test]
    fn test_pawn_white_one_step() {
        let pawn = Pawn::new(Color::White, Position::new(2, 3));
        // White pawn can move one square forward
        assert!(pawn.is_valid_basic_move(Position::new(3, 3), None));
    }

    #[test]
    fn test_pawn_white_two_steps_initial() {
        let pawn = Pawn::new(Color::White, Position::new(1, 3));
        // White pawn can move two squares from initial position (row 1)
        assert!(pawn.is_valid_basic_move(Position::new(3, 3), None));
    }

    #[test]
    fn test_pawn_black_one_step() {
        let pawn = Pawn::new(Color::Black, Position::new(5, 3));
        // Black pawn moves down (decreasing row)
        assert!(pawn.is_valid_basic_move(Position::new(4, 3), None));
    }

    #[test]
    fn test_pawn_black_two_steps_initial() {
        let pawn = Pawn::new(Color::Black, Position::new(6, 3));
        // Black pawn can move two squares from initial position (row 6)
        assert!(pawn.is_valid_basic_move(Position::new(4, 3), None));
    }

    #[test]
    fn test_pawn_invalid_backward() {
        let pawn_white = Pawn::new(Color::White, Position::new(3, 3));
        // Pawns cannot move backward
        assert!(!pawn_white.is_valid_basic_move(Position::new(2, 3), None));
    }

    #[test]
    fn test_pawn_white_diagonal_attack() {
        let pawn = Pawn::new(Color::White, Position::new(3, 3));
        // White pawn can attack diagonally forward
        assert!(pawn.is_valid_attack_move(Position::new(4, 4), None));
        assert!(pawn.is_valid_attack_move(Position::new(4, 2), None));
    }

    #[test]
    fn test_base_piece_data() {
        let rook = Rook::new(Color::White, Position::new(0, 0));
        let base = rook.base();
        
        assert_eq!(base.piece_type, PieceType::Rook);
        assert_eq!(base.color, Color::White);
        assert_eq!(base.position, Position::new(0, 0));
        
        let draw = base.get_draw();
        assert!(draw.contains("White"));
        assert!(draw.contains("R"));
    }

    #[test]
    fn test_variant_all_pieces() {
        // Test that we can create variants for all piece types
        let pieces: Vec<PieceVariant> = vec![
            PieceVariant::Rook(Rook::new(Color::White, Position::new(0, 0))),
            PieceVariant::Knight(Knight::new(Color::White, Position::new(0, 1))),
            PieceVariant::Bishop(Bishop::new(Color::White, Position::new(0, 2))),
            PieceVariant::Queen(Queen::new(Color::White, Position::new(0, 3))),
            PieceVariant::King(King::new(Color::White, Position::new(0, 4))),
            PieceVariant::Pawn(Pawn::new(Color::White, Position::new(1, 0))),
            PieceVariant::Empty(EmptyPiece::new(Position::new(4, 4))),
        ];
        
        // All variants implement Piece trait
        for piece in &pieces {
            let _ = piece.base();
            let _ = piece.is_valid_basic_move(Position::new(5, 5), None);
        }
        
        assert_eq!(pieces.len(), 7);
    }
}
