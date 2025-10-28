use crate::base_piece::{validate_move, BasePieceData, Piece};
use crate::types::{Color, PieceType, Position};

/// Rook implementation - maps to C++ Rook class
#[derive(Debug, Clone)]
pub struct Rook {
    base: BasePieceData,
}

impl Rook {
    pub fn new(color: Color, position: Position) -> Self {
        Self {
            base: BasePieceData::new(PieceType::Rook, color, position),
        }
    }

    /// Private helper analogous to C++ Rook::IsValidMove_
    /// Takes a closure for checking empty squares (like Square::IsEmptyAt)
    fn is_valid_move_with_check<F>(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
        is_empty: F,
    ) -> bool
    where
        F: Fn(Position) -> bool,
    {
        let d_row = to_position.row - self.base.position.row;
        let d_col = to_position.col - self.base.position.col;

        // Rook moves only horizontally or vertically
        if d_row != 0 && d_col != 0 {
            return false;
        }

        validate_move(
            d_row,
            d_col,
            to_position,
            validate_king_check,
            self.base.position,
            is_empty,
        )
    }
}

impl Piece for Rook {
    fn base(&self) -> &BasePieceData {
        &self.base
    }

    fn base_mut(&mut self) -> &mut BasePieceData {
        &mut self.base
    }

    fn is_valid_basic_move_impl(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool {
        // In real usage, you'd pass the actual board checker
        // For now we use a dummy that always returns true (no obstacles)
        self.is_valid_move_with_check(to_position, validate_king_check, |_| true)
    }

    fn is_valid_attack_move_impl(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_move_with_check(to_position, validate_king_check, |_| true)
    }
}

/// Knight implementation
#[derive(Debug, Clone)]
pub struct Knight {
    base: BasePieceData,
}

impl Knight {
    pub fn new(color: Color, position: Position) -> Self {
        Self {
            base: BasePieceData::new(PieceType::Knight, color, position),
        }
    }

    fn is_valid_move(&self, to_position: Position) -> bool {
        // Knight move offsets
        const DR: [i32; 8] = [-2, -1, 1, 2, 2, 1, -1, -2];
        const DC: [i32; 8] = [-1, -2, -2, -1, 1, 2, 2, 1];

        for i in 0..8 {
            let p = Position::new(
                self.base.position.row + DR[i],
                self.base.position.col + DC[i],
            );
            if p == to_position {
                return true;
            }
        }
        false
    }
}

impl Piece for Knight {
    fn base(&self) -> &BasePieceData {
        &self.base
    }

    fn base_mut(&mut self) -> &mut BasePieceData {
        &mut self.base
    }

    fn is_valid_basic_move_impl(
        &self,
        to_position: Position,
        _validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_move(to_position)
    }

    fn is_valid_attack_move_impl(
        &self,
        to_position: Position,
        _validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_move(to_position)
    }
}

/// Bishop implementation
#[derive(Debug, Clone)]
pub struct Bishop {
    base: BasePieceData,
}

impl Bishop {
    pub fn new(color: Color, position: Position) -> Self {
        Self {
            base: BasePieceData::new(PieceType::Bishop, color, position),
        }
    }

    fn is_valid_move_with_check<F>(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
        is_empty: F,
    ) -> bool
    where
        F: Fn(Position) -> bool,
    {
        let d_row = to_position.row - self.base.position.row;
        let d_col = to_position.col - self.base.position.col;

        // Bishop moves only diagonally
        if d_row.abs() != d_col.abs() {
            return false;
        }

        validate_move(
            d_row,
            d_col,
            to_position,
            validate_king_check,
            self.base.position,
            is_empty,
        )
    }
}

impl Piece for Bishop {
    fn base(&self) -> &BasePieceData {
        &self.base
    }

    fn base_mut(&mut self) -> &mut BasePieceData {
        &mut self.base
    }

    fn is_valid_basic_move_impl(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_move_with_check(to_position, validate_king_check, |_| true)
    }

    fn is_valid_attack_move_impl(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_move_with_check(to_position, validate_king_check, |_| true)
    }
}

/// Queen implementation - can move like rook + bishop
#[derive(Debug, Clone)]
pub struct Queen {
    base: BasePieceData,
}

impl Queen {
    pub fn new(color: Color, position: Position) -> Self {
        Self {
            base: BasePieceData::new(PieceType::Queen, color, position),
        }
    }

    fn is_valid_move_with_check<F>(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
        is_empty: F,
    ) -> bool
    where
        F: Fn(Position) -> bool,
    {
        let d_row = to_position.row - self.base.position.row;
        let d_col = to_position.col - self.base.position.col;

        // Queen moves horizontally, vertically, or diagonally
        if (d_row != 0 && d_col != 0) && (d_row.abs() != d_col.abs()) {
            return false;
        }

        validate_move(
            d_row,
            d_col,
            to_position,
            validate_king_check,
            self.base.position,
            is_empty,
        )
    }
}

impl Piece for Queen {
    fn base(&self) -> &BasePieceData {
        &self.base
    }

    fn base_mut(&mut self) -> &mut BasePieceData {
        &mut self.base
    }

    fn is_valid_basic_move_impl(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_move_with_check(to_position, validate_king_check, |_| true)
    }

    fn is_valid_attack_move_impl(
        &self,
        to_position: Position,
        validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_move_with_check(to_position, validate_king_check, |_| true)
    }
}

/// King implementation - moves one square in any direction
#[derive(Debug, Clone)]
pub struct King {
    base: BasePieceData,
}

impl King {
    pub fn new(color: Color, position: Position) -> Self {
        Self {
            base: BasePieceData::new(PieceType::King, color, position),
        }
    }

    fn is_valid_move(&self, to_position: Position) -> bool {
        // King can move one square in any direction (8 possible moves)
        const DR: [i32; 8] = [-1, 0, 1, 1, 1, 0, -1, -1];
        const DC: [i32; 8] = [-1, -1, -1, 0, 1, 1, 1, 0];

        for i in 0..8 {
            let p = Position::new(
                self.base.position.row + DR[i],
                self.base.position.col + DC[i],
            );
            if p == to_position {
                return true;
            }
        }
        false
    }
}

impl Piece for King {
    fn base(&self) -> &BasePieceData {
        &self.base
    }

    fn base_mut(&mut self) -> &mut BasePieceData {
        &mut self.base
    }

    fn is_valid_basic_move_impl(
        &self,
        to_position: Position,
        _validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_move(to_position)
    }

    fn is_valid_attack_move_impl(
        &self,
        to_position: Position,
        _validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_move(to_position)
    }
}

/// Pawn implementation - most complex movement rules
#[derive(Debug, Clone)]
pub struct Pawn {
    base: BasePieceData,
}

impl Pawn {
    pub fn new(color: Color, position: Position) -> Self {
        Self {
            base: BasePieceData::new(PieceType::Pawn, color, position),
        }
    }

    fn is_valid_basic_move_with_check<F>(
        &self,
        to_position: Position,
        is_empty: F,
    ) -> bool
    where
        F: Fn(Position) -> bool,
    {
        let d_row = to_position.row - self.base.position.row;
        let d_col = to_position.col - self.base.position.col;

        match self.base.color {
            Color::White => {
                // White pawns move up (increasing row)
                if d_col == 0 {
                    // One step forward
                    if d_row == 1 && is_empty(to_position) {
                        return true;
                    }
                    // Two steps from initial position (row 1)
                    if d_row == 2 
                        && self.base.position.row == 1 
                        && is_empty(to_position)
                        && is_empty(Position::new(self.base.position.row + 1, self.base.position.col))
                    {
                        return true;
                    }
                }
            }
            Color::Black => {
                // Black pawns move down (decreasing row)
                if d_col == 0 {
                    // One step forward
                    if d_row == -1 && is_empty(to_position) {
                        return true;
                    }
                    // Two steps from initial position (row 6)
                    if d_row == -2 
                        && self.base.position.row == 6 
                        && is_empty(to_position)
                        && is_empty(Position::new(self.base.position.row - 1, self.base.position.col))
                    {
                        return true;
                    }
                }
            }
        }
        false
    }

    fn is_valid_attack_move_with_check<F>(
        &self,
        to_position: Position,
        is_empty: F,
    ) -> bool
    where
        F: Fn(Position) -> bool,
    {
        let d_row = to_position.row - self.base.position.row;
        let d_col = to_position.col - self.base.position.col;

        match self.base.color {
            Color::White => {
                // White pawns attack diagonally forward
                if d_col.abs() == 1 && d_row == 1 {
                    // Normal diagonal capture or en passant
                    return !is_empty(to_position) || true; // Simplified - en passant logic
                }
            }
            Color::Black => {
                // Black pawns attack diagonally forward (down)
                if d_col.abs() == 1 && d_row == -1 {
                    return !is_empty(to_position) || true; // Simplified - en passant logic
                }
            }
        }
        false
    }
}

impl Piece for Pawn {
    fn base(&self) -> &BasePieceData {
        &self.base
    }

    fn base_mut(&mut self) -> &mut BasePieceData {
        &mut self.base
    }

    fn is_valid_basic_move_impl(
        &self,
        to_position: Position,
        _validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_basic_move_with_check(to_position, |_| true)
    }

    fn is_valid_attack_move_impl(
        &self,
        to_position: Position,
        _validate_king_check: Option<Position>,
    ) -> bool {
        self.is_valid_attack_move_with_check(to_position, |_| true)
    }
}

/// EmptyPiece - represents an empty square
#[derive(Debug, Clone)]
pub struct EmptyPiece {
    base: BasePieceData,
}

impl EmptyPiece {
    pub fn new(position: Position) -> Self {
        Self {
            // Empty pieces don't have a real color/type, using White as placeholder
            base: BasePieceData::new(PieceType::Pawn, Color::White, position),
        }
    }
}

impl Piece for EmptyPiece {
    fn base(&self) -> &BasePieceData {
        &self.base
    }

    fn base_mut(&mut self) -> &mut BasePieceData {
        &mut self.base
    }

    fn is_valid_basic_move_impl(
        &self,
        _to_position: Position,
        _validate_king_check: Option<Position>,
    ) -> bool {
        false
    }

    fn is_valid_attack_move_impl(
        &self,
        _to_position: Position,
        _validate_king_check: Option<Position>,
    ) -> bool {
        false
    }
}
