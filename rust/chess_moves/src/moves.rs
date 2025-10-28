use crate::move_trait::{remove_unwanted_chars, Move, SharedMoveData};
use crate::types::{string_to_piece_type, Color, MoveData, MoveType, Position};

/// BasicMove - represents a simple piece movement
/// Maps to C++ class BasicMove : public Move
#[derive(Debug, Clone)]
pub struct BasicMove {
    shared: SharedMoveData,
}

impl BasicMove {
    pub fn new(move_type: MoveType, color: Color, move_text: String, comment: String) -> Self {
        let mut basic = Self {
            shared: SharedMoveData::new(move_type, color, move_text, comment),
        };
        basic.compute_move_data();
        basic
    }

    fn compute_move_data(&mut self) {
        let mut move_str = self.shared.move_text.clone();
        remove_unwanted_chars(&mut move_str);

        if move_str.len() < 2 {
            return;
        }

        // Last 2 characters are destination (e.g., "e4")
        let dest_str = &move_str[move_str.len() - 2..];
        let dest_chars: Vec<char> = dest_str.chars().collect();
        
        self.shared.move_data.to_position = Position::new(
            (dest_chars[1] as i32) - ('1' as i32),
            (dest_chars[0] as i32) - ('a' as i32),
        );

        // Remaining part before destination
        let remain = &move_str[..move_str.len() - 2];
        let mut piece_type = "P".to_string();

        if !remain.is_empty() {
            let chars: Vec<char> = remain.chars().collect();
            piece_type = chars[0].to_string();

            // Parse source position hints
            for ch in &chars[1..] {
                if ch.is_ascii_digit() {
                    self.shared.move_data.from_position.row = (*ch as i32) - ('1' as i32);
                } else if ch.is_ascii_lowercase() {
                    self.shared.move_data.from_position.col = (*ch as i32) - ('a' as i32);
                }
            }
        }

        self.shared.move_data.piece_type = string_to_piece_type(&piece_type);
    }
}

impl Move for BasicMove {
    fn move_type(&self) -> MoveType {
        self.shared.move_type
    }

    fn move_text(&self) -> &str {
        &self.shared.move_text
    }

    fn comment(&self) -> &str {
        &self.shared.comment
    }

    fn move_data(&self) -> &MoveData {
        &self.shared.move_data
    }

    fn move_data_mut(&mut self) -> &mut MoveData {
        &mut self.shared.move_data
    }

    fn process_move(&mut self) {
        // In the real implementation, this would call Square::ProcessBasicMove
        // and Square::MovePiece
        println!(
            "Processing BasicMove: {} from ({},{}) to ({},{})",
            self.move_text(),
            self.shared.move_data.from_position.row,
            self.shared.move_data.from_position.col,
            self.shared.move_data.to_position.row,
            self.shared.move_data.to_position.col
        );
    }

    fn set_move(&mut self, move_text: String, comment: String) {
        self.shared.move_text = move_text;
        self.shared.comment = comment;
    }
}

/// AttackMove - represents a capture
/// Maps to C++ class AttackMove : public virtual Move
#[derive(Debug, Clone)]
pub struct AttackMove {
    shared: SharedMoveData,
}

impl AttackMove {
    pub fn new(move_type: MoveType, color: Color, move_text: String, comment: String) -> Self {
        let mut attack = Self {
            shared: SharedMoveData::new(move_type, color, move_text, comment),
        };
        attack.compute_move_data();
        attack
    }

    fn compute_move_data(&mut self) {
        let mut move_str = self.shared.move_text.clone();
        remove_unwanted_chars(&mut move_str);

        if move_str.len() < 3 {
            return;
        }

        // Last 2 characters are destination, before that is 'x'
        let dest_str = &move_str[move_str.len() - 2..];
        let dest_chars: Vec<char> = dest_str.chars().collect();
        
        self.shared.move_data.to_position = Position::new(
            (dest_chars[1] as i32) - ('1' as i32),
            (dest_chars[0] as i32) - ('a' as i32),
        );

        // Remaining part (exclude 'x')
        let remain = &move_str[..move_str.len() - 3];
        let mut piece_type = "P".to_string();

        if !remain.is_empty() {
            let chars: Vec<char> = remain.chars().collect();
            
            if chars[0].is_ascii_uppercase() {
                piece_type = chars[0].to_string();
                
                for ch in &chars[1..] {
                    if ch.is_ascii_digit() {
                        self.shared.move_data.from_position.row = (*ch as i32) - ('1' as i32);
                    } else if ch.is_ascii_lowercase() {
                        self.shared.move_data.from_position.col = (*ch as i32) - ('a' as i32);
                    }
                }
            } else {
                // Pawn capture: just the file
                self.shared.move_data.from_position.col = (chars[0] as i32) - ('a' as i32);
            }
        }

        self.shared.move_data.piece_type = string_to_piece_type(&piece_type);
    }
}

impl Move for AttackMove {
    fn move_type(&self) -> MoveType {
        self.shared.move_type
    }

    fn move_text(&self) -> &str {
        &self.shared.move_text
    }

    fn comment(&self) -> &str {
        &self.shared.comment
    }

    fn move_data(&self) -> &MoveData {
        &self.shared.move_data
    }

    fn move_data_mut(&mut self) -> &mut MoveData {
        &mut self.shared.move_data
    }

    fn process_move(&mut self) {
        println!(
            "Processing AttackMove: {} capturing at ({},{})",
            self.move_text(),
            self.shared.move_data.to_position.row,
            self.shared.move_data.to_position.col
        );
    }

    fn set_move(&mut self, move_text: String, comment: String) {
        self.shared.move_text = move_text;
        self.shared.comment = comment;
    }
}

/// CastlingMove - represents castling (O-O or O-O-O)
/// Maps to C++ class CastlingMove : public Move
#[derive(Debug, Clone)]
pub struct CastlingMove {
    shared: SharedMoveData,
}

impl CastlingMove {
    pub fn new(move_type: MoveType, color: Color, move_text: String, comment: String) -> Self {
        let mut castling = Self {
            shared: SharedMoveData::new(move_type, color, move_text, comment),
        };
        castling.compute_move_data();
        castling
    }

    fn compute_move_data(&mut self) {
        let is_kingside = self.shared.move_text == "O-O" || self.shared.move_text == "0-0";
        let base_row = if self.shared.move_data.color == Color::White {
            0
        } else {
            7
        };

        if is_kingside {
            // Kingside castling
            self.shared.move_data.king_source = Position::new(base_row, 4);
            self.shared.move_data.king_destination = Position::new(base_row, 6);
            self.shared.move_data.rook_source = Position::new(base_row, 7);
            self.shared.move_data.rook_destination = Position::new(base_row, 5);
        } else {
            // Queenside castling
            self.shared.move_data.king_source = Position::new(base_row, 4);
            self.shared.move_data.king_destination = Position::new(base_row, 2);
            self.shared.move_data.rook_source = Position::new(base_row, 0);
            self.shared.move_data.rook_destination = Position::new(base_row, 3);
        }
    }
}

impl Move for CastlingMove {
    fn move_type(&self) -> MoveType {
        self.shared.move_type
    }

    fn move_text(&self) -> &str {
        &self.shared.move_text
    }

    fn comment(&self) -> &str {
        &self.shared.comment
    }

    fn move_data(&self) -> &MoveData {
        &self.shared.move_data
    }

    fn move_data_mut(&mut self) -> &mut MoveData {
        &mut self.shared.move_data
    }

    fn process_move(&mut self) {
        println!(
            "Processing CastlingMove: {} - King: ({},{}) -> ({},{}), Rook: ({},{}) -> ({},{})",
            self.move_text(),
            self.shared.move_data.king_source.row,
            self.shared.move_data.king_source.col,
            self.shared.move_data.king_destination.row,
            self.shared.move_data.king_destination.col,
            self.shared.move_data.rook_source.row,
            self.shared.move_data.rook_source.col,
            self.shared.move_data.rook_destination.row,
            self.shared.move_data.rook_destination.col
        );
    }

    fn set_move(&mut self, move_text: String, comment: String) {
        self.shared.move_text = move_text;
        self.shared.comment = comment;
    }
}

/// PromotionMove - pawn promotion
/// Maps to C++ class PromotionMove : public virtual Move
#[derive(Debug, Clone)]
pub struct PromotionMove {
    shared: SharedMoveData,
}

impl PromotionMove {
    pub fn new(move_type: MoveType, color: Color, move_text: String, comment: String) -> Self {
        let mut promotion = Self {
            shared: SharedMoveData::new(move_type, color, move_text, comment),
        };
        promotion.compute_move_data();
        promotion
    }

    fn compute_move_data(&mut self) {
        let mut move_str = self.shared.move_text.clone();
        remove_unwanted_chars(&mut move_str);

        if move_str.len() < 4 {
            return; // e.g., "e8=Q" is minimum
        }

        // Find the '=' sign
        if let Some(equals_pos) = move_str.find('=') {
            let promotion_char = move_str.chars().nth(equals_pos + 1).unwrap_or('Q');
            self.shared.move_data.promotion_type = string_to_piece_type(&promotion_char.to_string());

            // Destination is before '='
            let dest_str = &move_str[equals_pos - 2..equals_pos];
            let dest_chars: Vec<char> = dest_str.chars().collect();
            
            self.shared.move_data.to_position = Position::new(
                (dest_chars[1] as i32) - ('1' as i32),
                (dest_chars[0] as i32) - ('a' as i32),
            );

            // Source file might be at the beginning
            if equals_pos > 2 {
                let first_char = move_str.chars().next().unwrap();
                if first_char.is_ascii_lowercase() {
                    self.shared.move_data.from_position.col = (first_char as i32) - ('a' as i32);
                }
            }
        }

        self.shared.move_data.piece_type = string_to_piece_type("P");
    }
}

impl Move for PromotionMove {
    fn move_type(&self) -> MoveType {
        self.shared.move_type
    }

    fn move_text(&self) -> &str {
        &self.shared.move_text
    }

    fn comment(&self) -> &str {
        &self.shared.comment
    }

    fn move_data(&self) -> &MoveData {
        &self.shared.move_data
    }

    fn move_data_mut(&mut self) -> &mut MoveData {
        &mut self.shared.move_data
    }

    fn process_move(&mut self) {
        println!(
            "Processing PromotionMove: {} promoting to {:?} at ({},{})",
            self.move_text(),
            self.shared.move_data.promotion_type,
            self.shared.move_data.to_position.row,
            self.shared.move_data.to_position.col
        );
    }

    fn set_move(&mut self, move_text: String, comment: String) {
        self.shared.move_text = move_text;
        self.shared.comment = comment;
    }
}

/// AttackPromotionMove - pawn captures and promotes
/// Maps to C++ class AttackPromotionMove : public AttackMove, public PromotionMove
/// 
/// KEY RUST PATTERN: Instead of multiple inheritance, we compose the behavior
#[derive(Debug, Clone)]
pub struct AttackPromotionMove {
    shared: SharedMoveData,
}

impl AttackPromotionMove {
    pub fn new(move_type: MoveType, color: Color, move_text: String, comment: String) -> Self {
        let mut attack_promo = Self {
            shared: SharedMoveData::new(move_type, color, move_text, comment),
        };
        attack_promo.compute_move_data();
        attack_promo
    }

    fn compute_move_data(&mut self) {
        let mut move_str = self.shared.move_text.clone();
        remove_unwanted_chars(&mut move_str);

        // e.g., "exd8=Q" or "fxe8=R"
        if let Some(equals_pos) = move_str.find('=') {
            let promotion_char = move_str.chars().nth(equals_pos + 1).unwrap_or('Q');
            self.shared.move_data.promotion_type = string_to_piece_type(&promotion_char.to_string());

            // Destination is before '='
            let dest_str = &move_str[equals_pos - 2..equals_pos];
            let dest_chars: Vec<char> = dest_str.chars().collect();
            
            self.shared.move_data.to_position = Position::new(
                (dest_chars[1] as i32) - ('1' as i32),
                (dest_chars[0] as i32) - ('a' as i32),
            );

            // Source file is at the beginning (before 'x')
            let first_char = move_str.chars().next().unwrap();
            if first_char.is_ascii_lowercase() {
                self.shared.move_data.from_position.col = (first_char as i32) - ('a' as i32);
            }
        }

        self.shared.move_data.piece_type = string_to_piece_type("P");
    }
}

impl Move for AttackPromotionMove {
    fn move_type(&self) -> MoveType {
        self.shared.move_type
    }

    fn move_text(&self) -> &str {
        &self.shared.move_text
    }

    fn comment(&self) -> &str {
        &self.shared.comment
    }

    fn move_data(&self) -> &MoveData {
        &self.shared.move_data
    }

    fn move_data_mut(&mut self) -> &mut MoveData {
        &mut self.shared.move_data
    }

    fn process_move(&mut self) {
        println!(
            "Processing AttackPromotionMove: {} capturing and promoting to {:?} at ({},{})",
            self.move_text(),
            self.shared.move_data.promotion_type,
            self.shared.move_data.to_position.row,
            self.shared.move_data.to_position.col
        );
    }

    fn set_move(&mut self, move_text: String, comment: String) {
        self.shared.move_text = move_text;
        self.shared.comment = comment;
    }
}
