use crate::move_variant::MoveVariant;

/// Round - represents a pair of moves (white and black)
/// Maps to C++ class Round
/// 
/// C++ Pattern:
/// ```cpp
/// class Round {
///     int roundIndex_;
///     std::string roundComment_;
///     std::unique_ptr<Move> whiteMove_;
///     std::unique_ptr<Move> blackMove_;
/// };
/// ```
/// 
/// Rust Pattern: Struct with owned MoveVariant
#[derive(Debug, Clone)]
pub struct Round {
    round_index: i32,
    round_comment: String,
    white_move: MoveVariant,
    black_move: MoveVariant,
}

impl Round {
    pub fn new(round_index: i32) -> Self {
        Self {
            round_index,
            round_comment: String::new(),
            white_move: MoveVariant::Undefined,
            black_move: MoveVariant::Undefined,
        }
    }

    pub fn with_moves(
        round_index: i32,
        round_comment: String,
        white_move: MoveVariant,
        black_move: MoveVariant,
    ) -> Self {
        Self {
            round_index,
            round_comment,
            white_move,
            black_move,
        }
    }

    pub fn round_index(&self) -> i32 {
        self.round_index
    }

    pub fn comment(&self) -> &str {
        &self.round_comment
    }

    pub fn white_move(&self) -> &MoveVariant {
        &self.white_move
    }

    pub fn black_move(&self) -> &MoveVariant {
        &self.black_move
    }

    pub fn white_move_mut(&mut self) -> &mut MoveVariant {
        &mut self.white_move
    }

    pub fn black_move_mut(&mut self) -> &mut MoveVariant {
        &mut self.black_move
    }

    /// Execute both moves in the round
    pub fn run(&mut self) {
        use crate::move_trait::Move;
        
        println!("Round {}", self.round_index);
        
        if !self.white_move.is_undefined() {
            print!("  White: ");
            self.white_move.process_move();
        }
        
        if !self.black_move.is_undefined() {
            print!("  Black: ");
            self.black_move.process_move();
        }
        
        if !self.round_comment.is_empty() {
            println!("  Comment: {}", self.round_comment);
        }
    }
}

/// Collection of rounds
/// Maps to C++ using Rounds = std::vector<Round>;
pub type Rounds = Vec<Round>;
