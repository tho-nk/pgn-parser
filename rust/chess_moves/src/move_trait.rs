use crate::types::{Color, MoveData, MoveType};

/// The Move trait replaces C++ virtual base class Move
/// 
/// C++ Pattern:
/// ```cpp
/// class Move {
/// public:
///     virtual void ProcessMove() {};
///     virtual ~Move() = default;
/// protected:
///     MoveType moveType_;
///     std::string moveText_;
///     MoveData moveData_;
/// };
/// ```
/// 
/// Rust Pattern: Trait with shared data struct
pub trait Move {
    /// Get the move type
    fn move_type(&self) -> MoveType;
    
    /// Get the move text (algebraic notation)
    fn move_text(&self) -> &str;
    
    /// Get the comment associated with this move
    fn comment(&self) -> &str;
    
    /// Get the move data
    fn move_data(&self) -> &MoveData;
    
    /// Get mutable move data
    fn move_data_mut(&mut self) -> &mut MoveData;
    
    /// Process/execute the move
    /// This is the virtual function that concrete types implement
    fn process_move(&mut self);
    
    /// Set move text and comment
    fn set_move(&mut self, move_text: String, comment: String);
}

/// SharedMoveData holds the common data for all move types
/// This replaces the protected members in C++ Move base class
#[derive(Debug, Clone)]
pub struct SharedMoveData {
    pub move_type: MoveType,
    pub move_text: String,
    pub comment: String,
    pub move_data: MoveData,
}

impl SharedMoveData {
    pub fn new(move_type: MoveType, color: Color, move_text: String, comment: String) -> Self {
        Self {
            move_type,
            move_text,
            comment,
            move_data: MoveData::new(color),
        }
    }
}

/// Helper function to remove unwanted characters
/// Maps to C++ helper::removeUnwantedChars(str)
pub fn remove_unwanted_chars(s: &mut String) {
    s.retain(|c| c != '+' && c != '#' && c != '!' && c != '?');
}
