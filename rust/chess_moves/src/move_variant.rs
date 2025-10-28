use crate::move_trait::Move;
use crate::moves::{AttackMove, AttackPromotionMove, BasicMove, CastlingMove, PromotionMove};
use crate::types::{Color, MoveData, MoveType};

/// MoveVariant - The Rust idiomatic way to handle polymorphic moves
/// 
/// C++ uses:
/// - std::unique_ptr<Move> with virtual dispatch
/// - Factory pattern to create different move types
/// 
/// Rust uses:
/// - enum with variants for each move type
/// - Implements Move trait directly on the enum
/// - Zero-cost abstraction with static dispatch
#[derive(Debug, Clone)]
pub enum MoveVariant {
    Basic(BasicMove),
    Attack(AttackMove),
    Castling(CastlingMove),
    Promotion(PromotionMove),
    AttackPromotion(AttackPromotionMove),
    Undefined,
}

impl MoveVariant {
    /// Factory method - replaces C++ move_factory::CreateMove()
    /// 
    /// C++ Pattern:
    /// ```cpp
    /// std::unique_ptr<Move> CreateMove(
    ///     const MoveType &moveType,
    ///     const Color &color,
    ///     std::string &&moveText,
    ///     std::string &&comment
    /// );
    /// ```
    /// 
    /// Rust Pattern: Associated function returning enum
    pub fn create(move_type: MoveType, color: Color, move_text: String, comment: String) -> Self {
        match move_type {
            MoveType::BasicMove => {
                MoveVariant::Basic(BasicMove::new(move_type, color, move_text, comment))
            }
            MoveType::AttackMove => {
                MoveVariant::Attack(AttackMove::new(move_type, color, move_text, comment))
            }
            MoveType::CastlingMove => {
                MoveVariant::Castling(CastlingMove::new(move_type, color, move_text, comment))
            }
            MoveType::PromotionMove => {
                MoveVariant::Promotion(PromotionMove::new(move_type, color, move_text, comment))
            }
            MoveType::AttackPromotionMove => MoveVariant::AttackPromotion(
                AttackPromotionMove::new(move_type, color, move_text, comment),
            ),
            MoveType::Undefined => MoveVariant::Undefined,
        }
    }

    /// Check if this is an undefined move
    pub fn is_undefined(&self) -> bool {
        matches!(self, MoveVariant::Undefined)
    }
}

/// Implement Move trait directly on the enum
/// This allows calling trait methods without pattern matching at call sites!
impl Move for MoveVariant {
    fn move_type(&self) -> MoveType {
        match self {
            MoveVariant::Basic(m) => m.move_type(),
            MoveVariant::Attack(m) => m.move_type(),
            MoveVariant::Castling(m) => m.move_type(),
            MoveVariant::Promotion(m) => m.move_type(),
            MoveVariant::AttackPromotion(m) => m.move_type(),
            MoveVariant::Undefined => MoveType::Undefined,
        }
    }

    fn move_text(&self) -> &str {
        match self {
            MoveVariant::Basic(m) => m.move_text(),
            MoveVariant::Attack(m) => m.move_text(),
            MoveVariant::Castling(m) => m.move_text(),
            MoveVariant::Promotion(m) => m.move_text(),
            MoveVariant::AttackPromotion(m) => m.move_text(),
            MoveVariant::Undefined => "",
        }
    }

    fn comment(&self) -> &str {
        match self {
            MoveVariant::Basic(m) => m.comment(),
            MoveVariant::Attack(m) => m.comment(),
            MoveVariant::Castling(m) => m.comment(),
            MoveVariant::Promotion(m) => m.comment(),
            MoveVariant::AttackPromotion(m) => m.comment(),
            MoveVariant::Undefined => "",
        }
    }

    fn move_data(&self) -> &MoveData {
        match self {
            MoveVariant::Basic(m) => m.move_data(),
            MoveVariant::Attack(m) => m.move_data(),
            MoveVariant::Castling(m) => m.move_data(),
            MoveVariant::Promotion(m) => m.move_data(),
            MoveVariant::AttackPromotion(m) => m.move_data(),
            MoveVariant::Undefined => {
                // Return a static undefined move data
                static UNDEFINED_DATA: MoveData = MoveData {
                    color: Color::Undefined,
                    from_position: crate::types::Position { row: -1, col: -1 },
                    to_position: crate::types::Position { row: -1, col: -1 },
                    piece_type: crate::types::PieceType::Undefined,
                    promotion_type: crate::types::PieceType::Undefined,
                    king_source: crate::types::Position { row: -1, col: -1 },
                    king_destination: crate::types::Position { row: -1, col: -1 },
                    rook_source: crate::types::Position { row: -1, col: -1 },
                    rook_destination: crate::types::Position { row: -1, col: -1 },
                };
                &UNDEFINED_DATA
            }
        }
    }

    fn move_data_mut(&mut self) -> &mut MoveData {
        match self {
            MoveVariant::Basic(m) => m.move_data_mut(),
            MoveVariant::Attack(m) => m.move_data_mut(),
            MoveVariant::Castling(m) => m.move_data_mut(),
            MoveVariant::Promotion(m) => m.move_data_mut(),
            MoveVariant::AttackPromotion(m) => m.move_data_mut(),
            MoveVariant::Undefined => {
                // This is a bit tricky - we need a mutable reference
                // In practice, you wouldn't mutate an undefined move
                panic!("Cannot get mutable data for undefined move")
            }
        }
    }

    fn process_move(&mut self) {
        match self {
            MoveVariant::Basic(m) => m.process_move(),
            MoveVariant::Attack(m) => m.process_move(),
            MoveVariant::Castling(m) => m.process_move(),
            MoveVariant::Promotion(m) => m.process_move(),
            MoveVariant::AttackPromotion(m) => m.process_move(),
            MoveVariant::Undefined => {}
        }
    }

    fn set_move(&mut self, move_text: String, comment: String) {
        match self {
            MoveVariant::Basic(m) => m.set_move(move_text, comment),
            MoveVariant::Attack(m) => m.set_move(move_text, comment),
            MoveVariant::Castling(m) => m.set_move(move_text, comment),
            MoveVariant::Promotion(m) => m.set_move(move_text, comment),
            MoveVariant::AttackPromotion(m) => m.set_move(move_text, comment),
            MoveVariant::Undefined => {}
        }
    }
}
