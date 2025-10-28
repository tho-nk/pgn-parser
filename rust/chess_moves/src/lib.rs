pub mod move_trait;
pub mod move_variant;
pub mod moves;
pub mod round;
pub mod types;

// Re-exports for convenience
pub use move_trait::{Move, SharedMoveData};
pub use move_variant::MoveVariant;
pub use moves::{AttackMove, AttackPromotionMove, BasicMove, CastlingMove, PromotionMove};
pub use round::{Round, Rounds};
pub use types::{Color, MoveData, MoveType, PieceType, Position};

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_basic_move_parsing() {
        let mv = BasicMove::new(
            MoveType::BasicMove,
            Color::White,
            "e4".to_string(),
            "Good opening".to_string(),
        );
        
        let data = mv.move_data();
        assert_eq!(data.to_position.row, 3); // '4' - '1' = 3
        assert_eq!(data.to_position.col, 4); // 'e' - 'a' = 4
        assert_eq!(data.piece_type, PieceType::Pawn);
    }

    #[test]
    fn test_knight_move_parsing() {
        let mv = BasicMove::new(
            MoveType::BasicMove,
            Color::White,
            "Nf3".to_string(),
            String::new(),
        );
        
        let data = mv.move_data();
        assert_eq!(data.to_position.row, 2); // '3' - '1' = 2
        assert_eq!(data.to_position.col, 5); // 'f' - 'a' = 5
        assert_eq!(data.piece_type, PieceType::Knight);
    }

    #[test]
    fn test_attack_move_parsing() {
        let mv = AttackMove::new(
            MoveType::AttackMove,
            Color::Black,
            "exd5".to_string(),
            String::new(),
        );
        
        let data = mv.move_data();
        assert_eq!(data.to_position.row, 4); // '5' - '1' = 4
        assert_eq!(data.to_position.col, 3); // 'd' - 'a' = 3
        assert_eq!(data.from_position.col, 4); // 'e' - 'a' = 4
        assert_eq!(data.piece_type, PieceType::Pawn);
    }

    #[test]
    fn test_castling_kingside() {
        let mv = CastlingMove::new(
            MoveType::CastlingMove,
            Color::White,
            "O-O".to_string(),
            String::new(),
        );
        
        let data = mv.move_data();
        assert_eq!(data.king_source, Position::new(0, 4));
        assert_eq!(data.king_destination, Position::new(0, 6));
        assert_eq!(data.rook_source, Position::new(0, 7));
        assert_eq!(data.rook_destination, Position::new(0, 5));
    }

    #[test]
    fn test_castling_queenside() {
        let mv = CastlingMove::new(
            MoveType::CastlingMove,
            Color::Black,
            "O-O-O".to_string(),
            String::new(),
        );
        
        let data = mv.move_data();
        assert_eq!(data.king_source, Position::new(7, 4));
        assert_eq!(data.king_destination, Position::new(7, 2));
        assert_eq!(data.rook_source, Position::new(7, 0));
        assert_eq!(data.rook_destination, Position::new(7, 3));
    }

    #[test]
    fn test_promotion_move() {
        let mv = PromotionMove::new(
            MoveType::PromotionMove,
            Color::White,
            "e8=Q".to_string(),
            String::new(),
        );
        
        let data = mv.move_data();
        assert_eq!(data.to_position, Position::new(7, 4));
        assert_eq!(data.promotion_type, PieceType::Queen);
        assert_eq!(data.piece_type, PieceType::Pawn);
    }

    #[test]
    fn test_attack_promotion_move() {
        let mv = AttackPromotionMove::new(
            MoveType::AttackPromotionMove,
            Color::Black,
            "exd1=Q".to_string(),
            String::new(),
        );
        
        let data = mv.move_data();
        assert_eq!(data.to_position, Position::new(0, 3));
        assert_eq!(data.from_position.col, 4); // 'e' column
        assert_eq!(data.promotion_type, PieceType::Queen);
    }

    #[test]
    fn test_move_variant_factory() {
        let mv = MoveVariant::create(
            MoveType::BasicMove,
            Color::White,
            "d4".to_string(),
            "Queen's pawn".to_string(),
        );
        
        assert_eq!(mv.move_text(), "d4");
        assert_eq!(mv.comment(), "Queen's pawn");
        assert_eq!(mv.move_type(), MoveType::BasicMove);
    }

    #[test]
    fn test_move_variant_trait_methods() {
        let mut mv = MoveVariant::create(
            MoveType::AttackMove,
            Color::Black,
            "Qxe4".to_string(),
            String::new(),
        );
        
        // Can call trait methods directly on the variant
        assert_eq!(mv.move_type(), MoveType::AttackMove);
        
        // Process the move
        mv.process_move();
    }

    #[test]
    fn test_round() {
        let white = MoveVariant::create(
            MoveType::BasicMove,
            Color::White,
            "e4".to_string(),
            String::new(),
        );
        
        let black = MoveVariant::create(
            MoveType::BasicMove,
            Color::Black,
            "e5".to_string(),
            String::new(),
        );
        
        let mut round = Round::with_moves(1, "Classic opening".to_string(), white, black);
        
        assert_eq!(round.round_index(), 1);
        assert_eq!(round.comment(), "Classic opening");
        
        round.run();
    }

    #[test]
    fn test_rounds_collection() {
        let mut rounds: Rounds = Vec::new();
        
        rounds.push(Round::with_moves(
            1,
            String::new(),
            MoveVariant::create(MoveType::BasicMove, Color::White, "e4".to_string(), String::new()),
            MoveVariant::create(MoveType::BasicMove, Color::Black, "e5".to_string(), String::new()),
        ));
        
        rounds.push(Round::with_moves(
            2,
            String::new(),
            MoveVariant::create(MoveType::BasicMove, Color::White, "Nf3".to_string(), String::new()),
            MoveVariant::create(MoveType::BasicMove, Color::Black, "Nc6".to_string(), String::new()),
        ));
        
        assert_eq!(rounds.len(), 2);
    }
}
