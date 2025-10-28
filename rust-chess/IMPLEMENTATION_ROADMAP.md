# Rust Chess PGN Parser - Implementation Roadmap

## 🎯 Goal: Think in Rust, Not Just Translate C++

This guide teaches you **idiomatic Rust patterns** for completing your chess engine, focusing on:
- Rust ownership model vs C++ memory management
- Trait-based design vs inheritance
- Error handling philosophy
- Module organization
- Testing culture

---

## 📚 Part 1: Understanding "Thinking in Rust"

### Core Mental Model Shifts

| C++ Thinking | Rust Thinking | Why |
|-------------|---------------|-----|
| "Who owns this pointer?" | "Who owns this value?" | Ownership is compiler-enforced |
| "Should I copy or reference?" | "Can I borrow or must I clone?" | Explicit about costs |
| "Inheritance hierarchy" | "Trait composition" | Favor composition over inheritance |
| "Try/catch exceptions" | "Result<T, E> everywhere" | Errors are values, not control flow |
| "Null pointer checks" | "Option<T> pattern match" | No null, only explicit absence |
| "Shared state with locks" | "Message passing or Arc<Mutex<T>>" | Fearless concurrency |

### The Rust Ownership Trinity

```rust
// 1. OWNERSHIP: Each value has exactly one owner
let board = Board::new();  // board OWNS the Board
// When board goes out of scope, memory is freed automatically

// 2. BORROWING: Temporary access without ownership
fn display_board(board: &Board) {  // Immutable borrow
    println!("{}", board);
}  // board goes back to caller

// 3. MUTABLE BORROWING: Exclusive write access
fn make_move(board: &mut Board, from: Position, to: Position) {
    board.move_piece(from, to).unwrap();
}  // Only one mutable borrow at a time!
```

**Key Rule**: Can have EITHER:
- Many immutable borrows (`&T`), OR
- One mutable borrow (`&mut T`)

But NEVER both at the same time!

---

## 🏗️ Part 2: Project Structure Philosophy

### Current Structure (Good Start!)
```
rust-chess/
├── Cargo.toml              # Workspace root
├── chess-common/           # Foundation: types, errors, parsing
├── chess-pieces/           # Domain: board state, pieces
├── chess-moves/            # Domain: move logic
└── chess-pgn-parser/       # Application: CLI binary
```

### Why This Structure?

**Bottom-Up Dependency Flow:**
```
chess-pgn-parser (binary)
    ↓ depends on
chess-pieces + chess-moves (domain logic)
    ↓ depends on
chess-common (foundation)
```

**Benefits:**
1. **Testability**: Each crate can be tested independently
2. **Reusability**: `chess-common` could be used in other chess projects
3. **Compilation Speed**: Only rebuild changed crates
4. **Clear Boundaries**: Each crate has a single responsibility

---

## 📦 Part 3: Module Organization Best Practices

### Pattern: Public API vs Internal Implementation

```rust
// chess-pieces/src/lib.rs (PUBLIC API)
pub mod board;      // Exposed
mod internal;       // Private to crate

// Re-export commonly used items
pub use board::Board;
pub use chess_common::{Position, Color, Result};

// Users can do:
// use chess_pieces::Board;
// Instead of:
// use chess_pieces::board::Board;
```

### Pattern: Feature-Based Modules

**Bad (C++ style - by type):**
```
src/
  ├── interfaces/   ❌ All traits
  ├── impl/         ❌ All implementations
  └── utils/        ❌ Grab bag
```

**Good (Rust style - by feature):**
```
src/
  ├── board.rs      ✅ Board type + all its methods
  ├── pieces.rs     ✅ All piece types together
  ├── moves.rs      ✅ All move types together
  └── validation.rs ✅ Move validation logic
```

---

## 🎨 Part 4: Implementing the Game Engine (Step-by-Step)

### Phase 1: Move String Parser (Week 1)

**File: `chess-moves/src/parser.rs`**

**Rust Thinking:** Use parser combinators or simple string matching

```rust
use chess_common::{Position, PieceType, MoveType, MoveData, Result, ChessError};

/// Parse algebraic notation to MoveData
/// Examples: "e4", "Nf3", "O-O", "exd5", "e8=Q"
pub fn parse_move(
    move_str: &str,
    color: Color,
) -> Result<MoveData> {
    let clean = move_str
        .chars()
        .filter(|c| !"+#!?".contains(*c))  // Remove annotations
        .collect::<String>();
    
    // Castling special case
    if clean == "O-O" {
        return Ok(MoveData {
            move_type: MoveType::Castling,
            is_kingside_castling: true,
            color,
            ..MoveData::new(color)
        });
    }
    
    // Pattern matching - very Rust!
    match parse_standard_move(&clean, color) {
        Ok(data) => Ok(data),
        Err(e) => Err(ChessError::ParseError(
            format!("Failed to parse '{}': {}", move_str, e)
        )),
    }
}

fn parse_standard_move(s: &str, color: Color) -> Result<MoveData> {
    let chars: Vec<char> = s.chars().collect();
    let mut data = MoveData::new(color);
    
    // Check for promotion (e8=Q)
    if let Some(eq_pos) = s.find('=') {
        data.promotion_type = Some(parse_piece_type(&s[eq_pos+1..])?);
        data.move_type = MoveType::Promotion;
    }
    
    // Check for capture (x)
    if s.contains('x') {
        data.move_type = if data.promotion_type.is_some() {
            MoveType::AttackPromotion
        } else {
            MoveType::Attack
        };
    }
    
    // Parse destination (last 2 chars like "e4")
    let dest_str = if s.contains('=') {
        &s[s.find('=').unwrap()-2..s.find('=').unwrap()]
    } else {
        &s[s.len()-2..]
    };
    data.to_position = Position::from_algebraic(dest_str)
        .ok_or_else(|| ChessError::ParseError(
            format!("Invalid destination: {}", dest_str)
        ))?;
    
    // Parse piece type (first char if uppercase)
    if chars[0].is_uppercase() {
        data.piece_type = match chars[0] {
            'N' => PieceType::Knight,
            'B' => PieceType::Bishop,
            'R' => PieceType::Rook,
            'Q' => PieceType::Queen,
            'K' => PieceType::King,
            _ => return Err(ChessError::ParseError(
                format!("Unknown piece: {}", chars[0])
            )),
        };
    } else {
        data.piece_type = PieceType::Pawn;
    }
    
    // Parse disambiguation (e.g., "Nbd7" - knight from b-file)
    // TODO: Extract file/rank hints into data.from_position
    
    Ok(data)
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_parse_pawn_move() {
        let data = parse_move("e4", Color::White).unwrap();
        assert_eq!(data.piece_type, PieceType::Pawn);
        assert_eq!(data.to_position, Position::from_algebraic("e4").unwrap());
    }
    
    #[test]
    fn test_parse_knight_move() {
        let data = parse_move("Nf3", Color::White).unwrap();
        assert_eq!(data.piece_type, PieceType::Knight);
    }
    
    #[test]
    fn test_parse_capture() {
        let data = parse_move("exd5", Color::White).unwrap();
        assert_eq!(data.move_type, MoveType::Attack);
    }
}
```

**Rust Best Practices Here:**
1. ✅ Return `Result<T, E>` for fallible operations
2. ✅ Use `&str` for string slices (no allocation)
3. ✅ Pattern matching instead of if-else chains
4. ✅ Comprehensive tests in same file
5. ✅ Descriptive error messages

---

### Phase 2: Connect Moves to Board (Week 2)

**File: `chess-moves/src/executor.rs`**

**Rust Thinking:** Trait defines behavior, implementation provides logic

```rust
use chess_common::{Result, MoveData, MoveType};
use chess_pieces::Board;

/// Trait for executing moves on a board
/// Think: "What can this move DO?" not "What IS this move?"
pub trait MoveExecutor {
    fn execute(&self, board: &mut Board) -> Result<()>;
}

/// Execute a parsed MoveData on the board
pub fn execute_move(data: &MoveData, board: &mut Board) -> Result<()> {
    // Board validates and finds the correct piece
    match data.move_type {
        MoveType::Basic => {
            let mut move_data = data.clone();
            board.process_basic_move(&mut move_data)?;
            board.move_piece(move_data.from_position, move_data.to_position)?;
        }
        
        MoveType::Attack => {
            let mut move_data = data.clone();
            board.process_attack_move(&mut move_data)?;
            board.attack_piece(move_data.from_position, move_data.to_position)?;
        }
        
        MoveType::Castling => {
            execute_castling(data, board)?;
        }
        
        MoveType::Promotion => {
            let mut move_data = data.clone();
            board.process_basic_move(&mut move_data)?;
            board.process_promotion(
                data.promotion_type.unwrap(),
                data.color,
                move_data.from_position,
                move_data.to_position,
            )?;
        }
        
        MoveType::AttackPromotion => {
            let mut move_data = data.clone();
            board.process_attack_move(&mut move_data)?;
            board.attack_piece(move_data.from_position, move_data.to_position)?;
            board.process_promotion(
                data.promotion_type.unwrap(),
                data.color,
                move_data.from_position,
                move_data.to_position,
            )?;
        }
    }
    
    Ok(())
}

fn execute_castling(data: &MoveData, board: &mut Board) -> Result<()> {
    let rank = if data.color == Color::White { 0 } else { 7 };
    
    if data.is_kingside_castling {
        // King e1 -> g1, Rook h1 -> f1
        board.move_piece(
            Position::new(rank, 4),  // King
            Position::new(rank, 6),
        )?;
        board.move_piece(
            Position::new(rank, 7),  // Rook
            Position::new(rank, 5),
        )?;
    } else {
        // Queenside: King e1 -> c1, Rook a1 -> d1
        board.move_piece(
            Position::new(rank, 4),
            Position::new(rank, 2),
        )?;
        board.move_piece(
            Position::new(rank, 0),
            Position::new(rank, 3),
        )?;
    }
    
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_execute_basic_move() {
        let mut board = Board::new();
        let data = MoveData {
            piece_type: PieceType::Pawn,
            color: Color::White,
            to_position: Position::from_algebraic("e4").unwrap(),
            move_type: MoveType::Basic,
            ..MoveData::new(Color::White)
        };
        
        execute_move(&data, &mut board).unwrap();
        // Verify move was made
        assert!(!board.is_empty(Position::from_algebraic("e4").unwrap()));
    }
}
```

**Rust Best Practices:**
1. ✅ Trait for polymorphism (not inheritance)
2. ✅ `&mut Board` - exclusive write access
3. ✅ Error propagation with `?` operator
4. ✅ Match exhaustiveness ensures all cases handled

---

### Phase 3: Round System (Week 2)

**File: `chess-moves/src/round.rs`**

```rust
use chess_common::{Color, Result};
use chess_pieces::Board;
use super::parser::parse_move;
use super::executor::execute_move;

/// A round consists of white's move and optionally black's move
pub struct Round {
    white_move: String,
    black_move: Option<String>,
}

impl Round {
    /// Parse a round string like "1. e4 e5" or "43. Rxd7"
    pub fn from_string(s: &str) -> Result<Self> {
        // Remove round number prefix
        let without_number = s.split('.').nth(1)
            .unwrap_or(s)
            .trim();
        
        let moves: Vec<&str> = without_number
            .split_whitespace()
            .collect();
        
        Ok(Round {
            white_move: moves.get(0)
                .ok_or_else(|| ChessError::ParseError(
                    "Missing white move".into()
                ))?
                .to_string(),
            black_move: moves.get(1).map(|s| s.to_string()),
        })
    }
    
    /// Execute both moves on the board
    pub fn execute(&self, board: &mut Board) -> Result<()> {
        // White's move
        let white_data = parse_move(&self.white_move, Color::White)?;
        execute_move(&white_data, board)?;
        
        // Black's move (if exists)
        if let Some(ref black_str) = self.black_move {
            let black_data = parse_move(black_str, Color::Black)?;
            execute_move(&black_data, board)?;
        }
        
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_parse_round() {
        let round = Round::from_string("1. e4 e5").unwrap();
        assert_eq!(round.white_move, "e4");
        assert_eq!(round.black_move, Some("e5".to_string()));
    }
    
    #[test]
    fn test_execute_round() {
        let mut board = Board::new();
        let round = Round::from_string("1. e4 e5").unwrap();
        round.execute(&mut board).unwrap();
        
        // Check both pawns moved
        assert!(!board.is_empty(Position::from_algebraic("e4").unwrap()));
        assert!(!board.is_empty(Position::from_algebraic("e5").unwrap()));
    }
}
```

**Rust Patterns:**
1. ✅ `Option<String>` for optional black move
2. ✅ `if let Some(ref x)` pattern for conditional execution
3. ✅ String allocation only when needed
4. ✅ Error context with descriptive messages

---

### Phase 4: Game Engine (Week 3)

**File: `chess-pgn-parser/src/game.rs`**

**Rust Thinking:** Separate concerns - parsing, state, execution

```rust
use chess_common::{Result, ChessError};
use chess_pieces::Board;
use chess_moves::Round;
use std::path::Path;

pub struct Game {
    board: Board,
    rounds: Vec<Round>,
    current_round: usize,
    result: Option<GameResult>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum GameResult {
    WhiteWins,
    BlackWins,
    Draw,
    Unknown,
}

impl Game {
    /// Load game from PGN file
    pub fn from_pgn_file(path: &Path) -> Result<Self> {
        // Parse PGN
        let parsing_result = chess_common::parse_file(path)?;
        
        // Convert to rounds
        let rounds = parsing_result
            .round_queue
            .into_iter()
            .map(|s| Round::from_string(&s))
            .collect::<Result<Vec<_>>>()?;
        
        // Parse result
        let result = Self::parse_result(&parsing_result.last_run);
        
        Ok(Self {
            board: Board::new(),
            rounds,
            current_round: 0,
            result: Some(result),
        })
    }
    
    fn parse_result(s: &str) -> GameResult {
        if s.contains("1-0") {
            GameResult::WhiteWins
        } else if s.contains("0-1") {
            GameResult::BlackWins
        } else if s.contains("1/2-1/2") {
            GameResult::Draw
        } else {
            GameResult::Unknown
        }
    }
    
    /// Execute all moves
    pub fn play(&mut self) -> Result<()> {
        for (i, round) in self.rounds.iter().enumerate() {
            self.current_round = i;
            
            // Execute round
            if let Err(e) = round.execute(&mut self.board) {
                return Err(ChessError::InvalidMove(
                    format!("Round {}: {}", i + 1, e)
                ));
            }
        }
        
        Ok(())
    }
    
    /// Execute moves one by one (for debugging/visualization)
    pub fn step(&mut self) -> Result<bool> {
        if self.current_round >= self.rounds.len() {
            return Ok(false); // Game over
        }
        
        self.rounds[self.current_round].execute(&mut self.board)?;
        self.current_round += 1;
        Ok(true)
    }
    
    /// Get current board state
    pub fn board(&self) -> &Board {
        &self.board
    }
    
    /// Display game info
    pub fn summary(&self) -> String {
        format!(
            "Rounds: {}\nCurrent: {}\nResult: {:?}\n\n{}",
            self.rounds.len(),
            self.current_round,
            self.result,
            self.board
        )
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_scholars_mate() {
        // 4-move checkmate
        let pgn = std::path::Path::new("tests/fixtures/scholars_mate.pgn");
        if pgn.exists() {
            let mut game = Game::from_pgn_file(pgn).unwrap();
            game.play().unwrap();
            assert_eq!(game.result, Some(GameResult::WhiteWins));
        }
    }
}
```

**Rust Best Practices:**
1. ✅ Separate data (`Board`) from behavior (`Game`)
2. ✅ Borrowing (`&Board`) vs ownership
3. ✅ Iterator chains (`.into_iter().map().collect()`)
4. ✅ Early return with `?` operator
5. ✅ Enum for game results (not strings)

---

### Phase 5: CLI Application (Week 3)

**File: `chess-pgn-parser/src/main.rs`**

**Rust Thinking:** Use ecosystem crates for common tasks

```rust
use chess_pgn_parser::game::Game;
use std::path::PathBuf;
use std::process;

// Add to Cargo.toml:
// clap = { version = "4.5", features = ["derive"] }
use clap::Parser;

/// Chess PGN Parser and Validator
#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    /// PGN file to parse
    #[arg(value_name = "FILE")]
    file: PathBuf,
    
    /// Show board after each move
    #[arg(short, long)]
    step: bool,
    
    /// Verbose output
    #[arg(short, long)]
    verbose: bool,
}

fn main() {
    let args = Args::parse();
    
    // Validate file exists
    if !args.file.exists() {
        eprintln!("Error: File not found: {}", args.file.display());
        process::exit(1);
    }
    
    // Load game
    let mut game = match Game::from_pgn_file(&args.file) {
        Ok(g) => g,
        Err(e) => {
            eprintln!("Error loading PGN: {}", e);
            process::exit(1);
        }
    };
    
    if args.verbose {
        println!("Loaded game from: {}", args.file.display());
        println!("{} rounds to play\n", game.rounds().len());
    }
    
    // Execute game
    if args.step {
        // Step-by-step mode
        let mut round_num = 1;
        while game.step().unwrap_or(false) {
            println!("=== After Round {} ===", round_num);
            println!("{}\n", game.board());
            round_num += 1;
            
            // Wait for user input
            println!("Press Enter to continue...");
            let mut input = String::new();
            std::io::stdin().read_line(&mut input).unwrap();
        }
    } else {
        // Play entire game
        if let Err(e) = game.play() {
            eprintln!("Error during game: {}", e);
            process::exit(1);
        }
    }
    
    // Display final state
    println!("=== Final Position ===");
    println!("{}", game.summary());
}

// chess-pgn-parser/src/lib.rs
pub mod game;

// Re-export for convenience
pub use game::Game;
```

**Update Cargo.toml:**
```toml
[package]
name = "chess-pgn-parser"
version.workspace = true
edition.workspace = true

[dependencies]
chess-common = { path = "../chess-common" }
chess-pieces = { path = "../chess-pieces" }
chess-moves = { path = "../chess-moves" }
clap = { version = "4.5", features = ["derive"] }

[[bin]]
name = "chess-pgn"
path = "src/main.rs"
```

**Rust Ecosystem Usage:**
1. ✅ `clap` for CLI parsing (derive API is very Rust!)
2. ✅ `PathBuf` for cross-platform paths
3. ✅ `process::exit()` for error codes
4. ✅ Pattern matching on `Result`

---

## 🧪 Part 5: Testing Strategy (The Rust Way)

### Test Organization

```
chess-moves/
├── src/
│   ├── lib.rs
│   ├── parser.rs      #[cfg(test)] mod tests { ... }
│   ├── executor.rs    #[cfg(test)] mod tests { ... }
│   └── round.rs       #[cfg(test)] mod tests { ... }
├── tests/             # Integration tests
│   ├── parse_moves.rs
│   └── execute_game.rs
└── benches/           # Benchmarks
    └── parsing.rs
```

### Unit Tests (in same file)

```rust
// In parser.rs
#[cfg(test)]
mod tests {
    use super::*;  // Import parent module
    
    #[test]
    fn test_basic_pawn_move() {
        let result = parse_move("e4", Color::White);
        assert!(result.is_ok());
        let data = result.unwrap();
        assert_eq!(data.piece_type, PieceType::Pawn);
    }
    
    #[test]
    #[should_panic(expected = "Invalid")]
    fn test_invalid_move() {
        parse_move("z9", Color::White).unwrap();
    }
}
```

### Integration Tests

**File: `chess-moves/tests/full_game.rs`**

```rust
use chess_pieces::Board;
use chess_moves::{Round, parse_move, execute_move};
use chess_common::Color;

#[test]
fn test_scholars_mate_sequence() {
    let mut board = Board::new();
    
    let moves = vec![
        ("e4", Color::White),
        ("e5", Color::Black),
        ("Bc4", Color::White),
        ("Nc6", Color::Black),
        ("Qh5", Color::White),
        ("Nf6", Color::Black),
        ("Qxf7", Color::White),  // Checkmate
    ];
    
    for (move_str, color) in moves {
        let data = parse_move(move_str, color).unwrap();
        execute_move(&data, &mut board).unwrap();
    }
    
    // Verify final position
    // King should be in check, etc.
}
```

### Property-Based Testing

**Add to Cargo.toml:**
```toml
[dev-dependencies]
proptest = "1.4"
```

```rust
use proptest::prelude::*;

proptest! {
    #[test]
    fn test_position_always_valid_after_parse(
        file in "[a-h]",
        rank in "[1-8]",
    ) {
        let pos_str = format!("{}{}", file, rank);
        let pos = Position::from_algebraic(&pos_str);
        assert!(pos.is_some());
        assert!(pos.unwrap().is_valid());
    }
    
    #[test]
    fn test_move_reversibility(
        from_row in 0i32..8,
        from_col in 0i32..8,
        to_row in 0i32..8,
        to_col in 0i32..8,
    ) {
        let mut board = Board::new();
        let from = Position::new(from_row, from_col);
        let to = Position::new(to_row, to_col);
        
        // Make move
        if board.move_piece(from, to).is_ok() {
            // Move back
            let result = board.move_piece(to, from);
            assert!(result.is_ok());
        }
    }
}
```

---

## 📊 Part 6: Error Handling Philosophy

### The Rust Way: Errors Are Values

```rust
// ❌ C++ way: exceptions
try {
    board.movePiece(from, to);
} catch (PgnException& e) {
    // Handle
}

// ✅ Rust way: Result type
match board.move_piece(from, to) {
    Ok(()) => { /* success */ }
    Err(e) => { /* handle error */ }
}

// Or more commonly with ?:
board.move_piece(from, to)?;  // Propagates error up
```

### Custom Error Type

**File: `chess-common/src/error.rs`** (Already done!)

```rust
use thiserror::Error;

#[derive(Error, Debug)]
pub enum ChessError {
    #[error("Invalid move: {0}")]
    InvalidMove(String),
    
    #[error("Parse error: {0}")]
    ParseError(String),
    
    #[error("IO error: {0}")]
    IoError(#[from] std::io::Error),
    
    // Add more as needed
    #[error("Game already finished")]
    GameFinished,
    
    #[error("Illegal position: {0}")]
    IllegalPosition(String),
}
```

### Error Context Pattern

```rust
use anyhow::Context; // Add: anyhow = "1.0" to Cargo.toml

fn load_game(path: &Path) -> anyhow::Result<Game> {
    let file = std::fs::read_to_string(path)
        .context("Failed to read PGN file")?;
    
    let result = parse_file(path)
        .context(format!("Failed to parse {}", path.display()))?;
    
    Ok(Game::from_parsing_result(result)
        .context("Failed to create game from PGN")?)
}
```

---

## 🔧 Part 7: Dependencies Philosophy

### Choosing Crates

**Rule of Thumb:**
- ✅ Use popular, well-maintained crates for common tasks
- ❌ Don't reinvent the wheel
- ✅ Check crates.io downloads, recent updates, documentation

### Recommended Additions

```toml
[dependencies]
# CLI
clap = { version = "4.5", features = ["derive"] }

# Error handling
thiserror = "1.0"   # For library errors
anyhow = "1.0"      # For application errors

# Serialization (if saving/loading games)
serde = { version = "1.0", features = ["derive"] }
serde_json = "1.0"

[dev-dependencies]
# Testing
proptest = "1.4"    # Property-based testing
criterion = "0.5"   # Benchmarking

# Test utilities
pretty_assertions = "1.4"
```

### Why These?

1. **clap**: Industry standard for CLIs, great derive macros
2. **thiserror**: Makes custom error types easy
3. **anyhow**: Great for application error handling
4. **serde**: When you want JSON/TOML export
5. **proptest**: Catches edge cases unit tests miss
6. **criterion**: Professional benchmarking

---

## 📁 Part 8: Final File Structure

```
rust-chess/
├── Cargo.toml (workspace)
│
├── chess-common/
│   ├── Cargo.toml
│   ├── src/
│   │   ├── lib.rs          # Public API
│   │   ├── types.rs        # Position, Color, etc. ✅
│   │   ├── error.rs        # ChessError ✅
│   │   └── parser.rs       # PGN file parsing ✅
│   └── tests/
│       └── parser_tests.rs
│
├── chess-pieces/
│   ├── Cargo.toml
│   ├── src/
│   │   ├── lib.rs          # Public API
│   │   ├── board.rs        # Board state ✅
│   │   ├── pieces.rs       # All pieces ✅
│   │   ├── variant.rs      # PieceVariant ✅
│   │   └── base_piece.rs   # Piece trait ✅
│   ├── tests/
│   │   └── board_tests.rs
│   └── examples/
│       └── board_demo.rs   # Show off the board
│
├── chess-moves/
│   ├── Cargo.toml
│   ├── src/
│   │   ├── lib.rs          # Public API
│   │   ├── parser.rs       # ⏳ Parse move strings
│   │   ├── executor.rs     # ⏳ Execute moves on board
│   │   └── round.rs        # ⏳ Round abstraction
│   ├── tests/
│   │   ├── parser_tests.rs
│   │   └── full_game.rs
│   └── benches/
│       └── move_parsing.rs
│
└── chess-pgn-parser/
    ├── Cargo.toml
    ├── src/
    │   ├── main.rs         # ⏳ CLI entry point
    │   ├── lib.rs          # Library interface
    │   └── game.rs         # ⏳ Game orchestrator
    ├── tests/
    │   └── cli_tests.rs
    └── fixtures/           # Test PGN files
        ├── scholars_mate.pgn
        └── sample_game.pgn
```

---

## 🎯 Part 9: Implementation Checklist

### Week 1: Move Parsing
- [ ] Create `chess-moves/src/parser.rs`
- [ ] Implement `parse_move()` for basic moves
- [ ] Add support for captures, castling
- [ ] Add support for promotions
- [ ] Write comprehensive tests (20+ test cases)
- [ ] Handle disambiguation (Nbd7 vs Nfd7)

### Week 2: Move Execution
- [ ] Create `chess-moves/src/executor.rs`
- [ ] Implement `execute_move()`
- [ ] Handle all move types
- [ ] Create `chess-moves/src/round.rs`
- [ ] Implement `Round::from_string()`
- [ ] Implement `Round::execute()`
- [ ] Integration tests with real games

### Week 3: Game Engine
- [ ] Create `chess-pgn-parser/src/game.rs`
- [ ] Implement `Game::from_pgn_file()`
- [ ] Implement `Game::play()`
- [ ] Implement `Game::step()` for debugging
- [ ] Add game result parsing
- [ ] Error recovery and reporting

### Week 4: CLI & Polish
- [ ] Implement CLI with `clap`
- [ ] Add `--step` mode
- [ ] Add `--verbose` flag
- [ ] Pretty-print boards
- [ ] Add progress indicator for long games
- [ ] Error messages with helpful hints

### Week 5: Testing & Docs
- [ ] Run against all pytest PGN files
- [ ] Property-based tests
- [ ] Benchmarks for performance
- [ ] Documentation with examples
- [ ] README with quick start
- [ ] CI/CD setup (GitHub Actions)

---

## 🧠 Part 10: "Think in Rust" Cheat Sheet

### When Translating C++ → Rust

| C++ Pattern | Rust Equivalent | Example |
|-------------|----------------|---------|
| `std::variant<A, B>` | `enum Variant { A(A), B(B) }` | PieceVariant |
| `std::unique_ptr<T>` | `Box<T>` | Heap allocation |
| `std::shared_ptr<T>` | `Arc<T>` | Shared ownership |
| `std::vector<T>` | `Vec<T>` | Dynamic array |
| `std::array<T, N>` | `[T; N]` | Fixed-size array |
| `std::optional<T>` | `Option<T>` | Nullable value |
| `try/catch` | `Result<T, E>` | Error handling |
| Virtual methods | Trait + `dyn Trait` | Polymorphism |
| Templates | Generics + traits | Type parameters |
| `const T&` | `&T` | Immutable reference |
| `T&` | `&mut T` | Mutable reference |
| `std::move` | Ownership transfer | Automatic |

### Common Mistakes to Avoid

1. **Fighting the Borrow Checker** ❌
   ```rust
   // Don't try to hold multiple &mut
   let a = &mut board.pieces[0];
   let b = &mut board.pieces[1]; // Error!
   ```
   
   **Solution**: Use indices or split_at_mut
   ```rust
   let piece_a = board.pieces[0].clone();
   let piece_b = &mut board.pieces[1];
   ```

2. **Cloning Too Much** ❌
   ```rust
   fn process(board: Board) { ... } // Takes ownership!
   board.move_piece(...); // Error: board moved
   ```
   
   **Solution**: Borrow when possible
   ```rust
   fn process(board: &Board) { ... }
   ```

3. **String Confusion** ❌
   ```rust
   fn parse(s: String) { ... } // Takes ownership
   parse(my_string);
   parse(my_string); // Error: moved
   ```
   
   **Solution**: Use &str
   ```rust
   fn parse(s: &str) { ... }
   ```

---

## 🚀 Getting Started Commands

```bash
# 1. Update chess-moves
cd chess-moves
cargo add chess-common --path ../chess-common
cargo add chess-pieces --path ../chess-pieces

# 2. Create new files
touch src/parser.rs src/executor.rs src/round.rs

# 3. Update lib.rs
echo "pub mod parser;" >> src/lib.rs
echo "pub mod executor;" >> src/lib.rs  
echo "pub mod round;" >> src/lib.rs

# 4. Run tests as you develop
cargo test --lib
cargo test --test parse_moves

# 5. Add CLI dependencies
cd ../chess-pgn-parser
cargo add clap --features derive
cargo add anyhow

# 6. Create game module
touch src/game.rs
echo "pub mod game;" > src/lib.rs

# 7. Build everything
cd ..
cargo build --workspace

# 8. Run your PGN parser!
cargo run --bin chess-pgn -- ../pytest/Abdusattorov.pgn
```

---

## 📚 Learning Resources

- **The Rust Book**: https://doc.rust-lang.org/book/
- **Rust by Example**: https://doc.rust-lang.org/rust-by-example/
- **Rust API Guidelines**: https://rust-lang.github.io/api-guidelines/
- **Error Handling**: https://nick.groenen.me/posts/rust-error-handling/
- **Crate Selection**: https://blessed.rs/

---

## 🎓 Key Takeaways

1. **Ownership** is your friend - it prevents memory bugs at compile time
2. **Borrow** when you can, **clone** when you must
3. **Result<T, E>** makes error handling explicit and composable
4. **Traits** enable polymorphism without inheritance overhead
5. **Enums** are powerful - use them liberally
6. **Tests** are first-class - write them in the same file
7. **Dependencies** are fine - use the ecosystem
8. **Compiler errors** are helpful - read them carefully

---

## ✨ Next Steps

Start with **Week 1: Move Parser**. This is the foundation for everything else.

```bash
cd chess-moves/src
vim parser.rs  # Start here!
```

Then test incrementally:
```bash
cargo test parser
```

Build iteratively - Rust's compiler will guide you! 🦀

Good luck! Feel free to ask questions as you implement each phase.
