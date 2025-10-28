# Starting the Chess PGN Parser in Rust - From Scratch

## 📋 Project Overview

**Goal**: Port a C++ chess PGN parser to idiomatic Rust

**C++ Architecture Analysis**:
```
pgn_parser (executable)
├── piece library (chess board state, pieces)
│   └── Square (singleton board manager)
├── move library (move types and execution)
│   └── Round (white + black move pair)
└── common library (parsing, types)
    ├── ParsingHelper (PGN file parser)
    └── Types (Color, Position, PieceType, MoveData)
```

**Entry Point**: `main.cpp` → Load PGN file → Square::LoadPGN() → Square::Run() → Process moves

---

## 🎯 Step 1: Project Structure (Best Practice)

### Option A: Cargo Workspace (RECOMMENDED for this project)

**Why**: Your C++ project has multiple libraries (piece, move, common). A workspace mirrors this structure perfectly.

```bash
cd /home/ugitho/workspace/parser-pgn
mkdir -p rust-chess-pgn
cd rust-chess-pgn

# Create workspace
cargo new --lib chess-common
cargo new --lib chess-pieces  
cargo new --lib chess-moves
cargo new chess-pgn-parser  # binary crate

# Create workspace Cargo.toml
cat > Cargo.toml << 'EOF'
[workspace]
members = [
    "chess-common",
    "chess-pieces",
    "chess-moves", 
    "chess-pgn-parser"
]

resolver = "2"

[workspace.package]
version = "0.1.0"
edition = "2021"
authors = ["Your Name <email@example.com>"]
license = "MIT OR Apache-2.0"

[workspace.dependencies]
# Shared dependencies across workspace
thiserror = "1.0"
EOF
```

**Dependency Graph**:
```
chess-pgn-parser (bin)
├── chess-pieces
│   ├── chess-common
│   └── chess-moves
├── chess-moves
│   └── chess-common
└── chess-common (types, errors, parsing)
```

### Option B: Single Crate with Modules (simpler, less modular)

```bash
cargo new chess-pgn-parser
cd chess-pgn-parser
mkdir -p src/{common,pieces,moves}
```

**Recommendation**: Use **Option A (Workspace)** - it matches your C++ structure and allows parallel development.

---

## 🏗️ Step 2: Bottom-Up Implementation Order

### Phase 1: Foundation (Week 1)

**1.1 Common Types** (`chess-common/src/lib.rs`)

Start here because everything depends on these types:

```rust
// chess-common/src/types.rs
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Color {
    White,
    Black,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Position {
    pub row: u8,  // 0-7 (or 1-8 for display)
    pub col: u8,  // 0-7
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum MoveType {
    Basic,
    Attack,
    Promotion,
    Castling,
    AttackPromotion,
}
```

**Why first**: No dependencies, used everywhere.

**1.2 Error Types** (`chess-common/src/error.rs`)

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
    
    #[error("Invalid position: row={0}, col={1}")]
    InvalidPosition(u8, u8),
}

pub type Result<T> = std::result::Result<T, ChessError>;
```

**Why second**: Establishes error handling patterns for the whole project.

**1.3 PGN Parser** (`chess-common/src/parser.rs`)

Port `ParsingHelper` - you already have this in `rust/pgn_parser/`! Just integrate it.

```rust
// Copy from rust/pgn_parser/src/parsing.rs
pub mod parser;
```

**Deliverable**: `cargo test` in `chess-common` passes all tests.

---

### Phase 2: Pieces (Week 2)

**2.1 Piece Trait** (`chess-pieces/src/trait.rs`)

```rust
use chess_common::{Position, Color, PieceType};

pub trait Piece {
    fn color(&self) -> Color;
    fn piece_type(&self) -> PieceType;
    fn position(&self) -> Position;
    
    /// Check if move is valid (without board state)
    fn is_valid_move(&self, to: Position) -> bool;
    
    /// Check if move is valid (with board state check)
    fn is_valid_move_with_check<F>(&self, to: Position, is_occupied: F) -> bool
    where
        F: Fn(Position) -> bool;
}
```

**2.2 Concrete Pieces**

Port from `rust/pieces/` - you already have Rook, Knight, Bishop, Queen, King, Pawn!

**2.3 Piece Enum** (idiomatic Rust pattern)

```rust
#[derive(Debug, Clone)]
pub enum PieceVariant {
    King(King),
    Queen(Queen),
    Rook(Rook),
    Bishop(Bishop),
    Knight(Knight),
    Pawn(Pawn),
    Empty,
}

impl Piece for PieceVariant {
    fn color(&self) -> Color {
        match self {
            Self::King(p) => p.color(),
            Self::Queen(p) => p.color(),
            // ... delegate to inner type
            Self::Empty => Color::White, // or panic
        }
    }
    // ... implement other methods
}
```

**Deliverable**: `cargo test` for all piece movements.

---

### Phase 3: Board State (Week 3)

**3.1 Board/Square** (`chess-pieces/src/board.rs`)

This is your C++ `Square` class - the core game state:

```rust
use chess_common::{Position, Color, PieceType, Result};

pub struct Board {
    // 8x8 board
    pieces: [[PieceVariant; 8]; 8],
    // En passant target square
    en_passant: Option<Position>,
}

impl Board {
    pub fn new() -> Self {
        Self {
            pieces: Self::initial_position(),
            en_passant: None,
        }
    }
    
    fn initial_position() -> [[PieceVariant; 8]; 8] {
        // Set up standard chess starting position
        // Row 0: white pieces (Rook, Knight, Bishop, Queen, King, ...)
        // Row 1: white pawns
        // Row 2-5: empty
        // Row 6: black pawns
        // Row 7: black pieces
        todo!()
    }
    
    pub fn get_piece(&self, pos: Position) -> &PieceVariant {
        &self.pieces[pos.row as usize][pos.col as usize]
    }
    
    pub fn get_piece_mut(&mut self, pos: Position) -> &mut PieceVariant {
        &mut self.pieces[pos.row as usize][pos.col as usize]
    }
    
    pub fn is_empty(&self, pos: Position) -> bool {
        matches!(self.get_piece(pos), PieceVariant::Empty)
    }
    
    pub fn move_piece(&mut self, from: Position, to: Position) -> Result<()> {
        // Validation
        // Execute move
        // Update state
        todo!()
    }
}
```

**Deliverable**: Board initialization and basic move execution.

---

### Phase 4: Moves (Week 4)

**4.1 Move Trait & Types** (`chess-moves/src/trait.rs`)

Port from `rust/chess_moves/` - you already have this!

```rust
pub trait Move {
    fn execute(&self, board: &mut Board) -> Result<()>;
    fn from(&self) -> Position;
    fn to(&self) -> Position;
    fn move_type(&self) -> MoveType;
}
```

**4.2 Move Enum**

```rust
pub enum MoveVariant {
    Basic(BasicMove),
    Attack(AttackMove),
    Castling(CastlingMove),
    Promotion(PromotionMove),
    AttackPromotion(AttackPromotionMove),
}

impl Move for MoveVariant {
    fn execute(&self, board: &mut Board) -> Result<()> {
        match self {
            Self::Basic(m) => m.execute(board),
            Self::Attack(m) => m.execute(board),
            // ...
        }
    }
}
```

**4.3 Round System**

```rust
pub struct Round {
    white_move: MoveVariant,
    black_move: Option<MoveVariant>,
}

impl Round {
    pub fn execute(&self, board: &mut Board) -> Result<()> {
        self.white_move.execute(board)?;
        if let Some(ref black) = self.black_move {
            black.execute(board)?;
        }
        Ok(())
    }
}
```

**Deliverable**: Parse move strings and execute on board.

---

### Phase 5: Integration (Week 5)

**5.1 Game Engine** (`chess-pgn-parser/src/game.rs`)

This replaces C++ `Square` as the main orchestrator:

```rust
use chess_common::parser::{parse_file, ParsingResult};
use chess_pieces::Board;
use chess_moves::{Round, MoveVariant};
use std::path::Path;

pub struct Game {
    board: Board,
    rounds: Vec<Round>,
    current_round: usize,
}

impl Game {
    pub fn from_pgn(path: &Path) -> Result<Self> {
        // Parse PGN file
        let parsing_result = parse_file(path)?;
        
        // Convert to rounds
        let rounds = Self::parse_rounds(parsing_result)?;
        
        Ok(Self {
            board: Board::new(),
            rounds,
            current_round: 0,
        })
    }
    
    fn parse_rounds(result: ParsingResult) -> Result<Vec<Round>> {
        // Convert PGN round strings to Round objects
        // "1. e4 e5" -> Round { white: BasicMove(e4), black: BasicMove(e5) }
        todo!()
    }
    
    pub fn run(&mut self) -> Result<()> {
        for round in &self.rounds {
            round.execute(&mut self.board)?;
            self.current_round += 1;
        }
        Ok(())
    }
    
    pub fn display(&self) -> String {
        // Pretty-print board state
        self.board.to_string()
    }
}
```

**5.2 Main Binary** (`chess-pgn-parser/src/main.rs`)

```rust
use chess_pgn_parser::Game;
use std::env;
use std::path::Path;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args: Vec<String> = env::args().collect();
    
    if args.len() != 2 {
        eprintln!("Usage: {} <pgn_file>", args[0]);
        std::process::exit(1);
    }
    
    let path = Path::new(&args[1]);
    if !path.exists() {
        eprintln!("File not found: {}", path.display());
        std::process::exit(1);
    }
    
    let mut game = Game::from_pgn(path)?;
    game.run()?;
    println!("{}", game.display());
    
    Ok(())
}
```

**Deliverable**: End-to-end PGN parsing and execution.

---

## 🛠️ Step 3: Development Best Practices

### 3.1 Cargo.toml Configuration

**chess-common/Cargo.toml**:
```toml
[package]
name = "chess-common"
version.workspace = true
edition.workspace = true

[dependencies]
thiserror.workspace = true

[dev-dependencies]
# For property-based testing
proptest = "1.4"
```

**chess-pieces/Cargo.toml**:
```toml
[package]
name = "chess-pieces"
version.workspace = true
edition.workspace = true

[dependencies]
chess-common = { path = "../chess-common" }

[dev-dependencies]
proptest = "1.4"
```

**chess-moves/Cargo.toml**:
```toml
[package]
name = "chess-moves"
version.workspace = true
edition.workspace = true

[dependencies]
chess-common = { path = "../chess-common" }
chess-pieces = { path = "../chess-pieces" }
```

**chess-pgn-parser/Cargo.toml**:
```toml
[package]
name = "chess-pgn-parser"
version.workspace = true
edition.workspace = true

[dependencies]
chess-common = { path = "../chess-common" }
chess-pieces = { path = "../chess-pieces" }
chess-moves = { path = "../chess-moves" }

# CLI utilities
clap = { version = "4.5", features = ["derive"] }

[dev-dependencies]
criterion = "0.5"  # Benchmarking

[[bench]]
name = "pgn_parsing"
harness = false
```

### 3.2 Testing Strategy

**Unit Tests** (in each module):
```rust
#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_rook_movement() {
        let rook = Rook::new(Color::White, Position::new(0, 0));
        assert!(rook.is_valid_move(Position::new(0, 7))); // horizontal
        assert!(rook.is_valid_move(Position::new(7, 0))); // vertical
        assert!(!rook.is_valid_move(Position::new(1, 1))); // diagonal - invalid
    }
}
```

**Integration Tests** (`tests/` directory):
```rust
// chess-pgn-parser/tests/integration_test.rs
use chess_pgn_parser::Game;
use std::path::Path;

#[test]
fn test_scholars_mate() {
    let path = Path::new("tests/fixtures/scholars_mate.pgn");
    let mut game = Game::from_pgn(path).unwrap();
    game.run().unwrap();
    // Assert checkmate state
}
```

**Property-Based Tests**:
```rust
use proptest::prelude::*;

proptest! {
    #[test]
    fn test_position_always_in_bounds(row in 0u8..8, col in 0u8..8) {
        let pos = Position::new(row, col);
        assert!(pos.is_valid());
    }
}
```

### 3.3 Documentation

Add to each crate:

```rust
//! # Chess Common Library
//!
//! Core types and utilities for chess game logic.
//!
//! ## Examples
//!
//! ```rust
//! use chess_common::Position;
//!
//! let pos = Position::new(0, 0);  // a1 in chess notation
//! assert!(pos.is_valid());
//! ```

/// Check if a position is on the board
///
/// # Examples
///
/// ```
/// # use chess_common::Position;
/// let pos = Position::new(0, 0);
/// assert!(pos.is_valid());
/// ```
pub fn is_valid(&self) -> bool {
    self.row < 8 && self.col < 8
}
```

Generate docs with: `cargo doc --open --workspace`

### 3.4 Continuous Integration

Create `.github/workflows/rust.yml`:

```yaml
name: Rust CI

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - uses: dtolnay/rust-toolchain@stable
      - run: cargo test --workspace
      - run: cargo clippy --workspace -- -D warnings
      - run: cargo fmt --check
  
  coverage:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - uses: dtolnay/rust-toolchain@stable
      - run: cargo install cargo-tarpaulin
      - run: cargo tarpaulin --workspace --out Xml
```

### 3.5 Performance Tools

**Benchmarking**:
```rust
// benches/pgn_parsing.rs
use criterion::{criterion_group, criterion_main, Criterion};
use chess_common::parser::parse_file;

fn benchmark_parse_large_pgn(c: &mut Criterion) {
    c.bench_function("parse_1000_moves", |b| {
        b.iter(|| {
            parse_file(Path::new("benches/fixtures/large_game.pgn"))
        })
    });
}

criterion_group!(benches, benchmark_parse_large_pgn);
criterion_main!(benches);
```

Run with: `cargo bench`

**Profiling**:
```bash
# Install flamegraph
cargo install flamegraph

# Profile your binary
cargo flamegraph --bin chess-pgn-parser -- tests/fixtures/game.pgn
```

---

## 📊 Step 4: Migration Checklist

Track progress as you port:

```markdown
### Common Module
- [x] Color enum
- [x] Position struct
- [x] PieceType enum
- [x] MoveType enum
- [x] MoveData struct
- [x] Error types
- [x] PGN parser (is_balanced, parse_file, etc.)
- [ ] Helper utilities

### Pieces Module
- [x] Piece trait
- [x] Rook
- [x] Knight
- [x] Bishop
- [x] Queen
- [x] King
- [x] Pawn
- [x] EmptyPiece
- [x] PieceVariant enum
- [ ] Board/Square
- [ ] Initial position setup
- [ ] Move validation with board state

### Moves Module
- [x] Move trait
- [x] BasicMove
- [x] AttackMove
- [x] CastlingMove
- [x] PromotionMove
- [x] AttackPromotionMove
- [x] MoveVariant enum
- [x] Round struct
- [ ] Move parsing from algebraic notation
- [ ] Move execution on board

### Integration
- [ ] Game struct
- [ ] PGN loading
- [ ] Round execution
- [ ] Board display
- [ ] CLI interface
- [ ] Error handling
- [ ] Tests for full games

### Quality
- [ ] Documentation (all pub items)
- [ ] Unit tests (>80% coverage)
- [ ] Integration tests (5+ PGN files)
- [ ] Benchmarks
- [ ] CI/CD pipeline
- [ ] README with examples
```

---

## 🚀 Step 5: Quick Start Commands

```bash
# Create workspace
mkdir rust-chess-pgn && cd rust-chess-pgn

# Initialize workspace
cargo new --lib chess-common
cargo new --lib chess-pieces
cargo new --lib chess-moves
cargo new chess-pgn-parser

# Create workspace Cargo.toml (see above)

# Migrate existing work
cp -r ../rust/pgn_parser/src/parsing.rs chess-common/src/parser.rs
cp -r ../rust/pieces/src/* chess-pieces/src/
cp -r ../rust/chess_moves/src/* chess-moves/src/

# Build and test
cargo build --workspace
cargo test --workspace
cargo clippy --workspace
cargo fmt

# Run
cargo run --bin chess-pgn-parser -- ../pytest/Abdusattorov.pgn
```

---

## 🎓 Key Rust Patterns to Use

1. **Error Handling**: Use `Result<T, ChessError>` everywhere, `?` operator
2. **Ownership**: Board owns pieces, moves borrow board mutably
3. **Enums for Polymorphism**: `PieceVariant`, `MoveVariant` instead of trait objects
4. **Traits for Behavior**: `Piece`, `Move` traits for common interface
5. **Option for Nullability**: `Option<Position>` for en passant instead of sentinel values
6. **Iterators**: Use `.iter()`, `.map()`, `.filter()` instead of loops
7. **Pattern Matching**: `match` instead of if-else chains
8. **Type Safety**: Newtype pattern for `Position`, `FromPosition`, `ToPosition`

---

## 📚 Next Steps

1. **Today**: Set up workspace structure
2. **Day 2-3**: Port common types and parser
3. **Day 4-7**: Integrate pieces (you already have implementations!)
4. **Week 2**: Build Board/Square with move execution
5. **Week 3**: Integrate moves and rounds
6. **Week 4**: CLI and end-to-end testing
7. **Week 5**: Documentation and polish

---

## 🔗 Resources

- [Rust Book](https://doc.rust-lang.org/book/)
- [Rust By Example](https://doc.rust-lang.org/rust-by-example/)
- [Cargo Workspaces](https://doc.rust-lang.org/book/ch14-03-cargo-workspaces.html)
- [Error Handling in Rust](https://doc.rust-lang.org/book/ch09-00-error-handling.html)
- [Chess Programming Wiki](https://www.chessprogramming.org/)

Good luck! 🦀
