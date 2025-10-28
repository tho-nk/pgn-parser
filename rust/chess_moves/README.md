# Chess Moves - Complete Rust Port

This is a **complete Rust port** of the C++ chess move implementation from `src/chess/move/`.

## What's Implemented

✅ **All 5 Move Types**: BasicMove, AttackMove, CastlingMove, PromotionMove, AttackPromotionMove  
✅ **Trait-based Polymorphism**: Replaces C++ virtual inheritance  
✅ **Factory Pattern**: `MoveVariant::create()` replaces `move_factory::CreateMove()`  
✅ **Enum Variants**: Zero-cost abstraction instead of `std::unique_ptr<Move>`  
✅ **Rounds System**: Complete game state with move pairs  
✅ **Comprehensive Tests**: Move parsing and execution for all types  
✅ **Working Example**: `examples/move_demo.rs` demonstrates all patterns

## Architecture Comparison

### C++ (Your Code)
```cpp
// Virtual base class
class Move {
public:
    virtual void ProcessMove() {}
    virtual ~Move() = default;
protected:
    MoveType moveType_;
    MoveData moveData_;
};

// Derived classes
class BasicMove : public Move { ... };
class AttackMove : public virtual Move { ... };
class PromotionMove : public virtual Move { ... };
class AttackPromotionMove : public AttackMove, public PromotionMove { ... };

// Factory
std::unique_ptr<Move> CreateMove(MoveType type, ...);

// Usage
std::unique_ptr<Move> move = move_factory::CreateMove(...);
move->ProcessMove();
```

### Rust (This Port)
```rust
// Trait (interface)
pub trait Move {
    fn process_move(&mut self);
    fn move_data(&self) -> &MoveData;
    // ...
}

// Concrete types
pub struct BasicMove { shared: SharedMoveData }
pub struct AttackMove { shared: SharedMoveData }
// ... all implement Move trait

// Enum for polymorphism
pub enum MoveVariant {
    Basic(BasicMove),
    Attack(AttackMove),
    // ...
}

impl Move for MoveVariant { /* delegates to inner type */ }

// Factory (associated function)
let mv = MoveVariant::create(MoveType::BasicMove, ...);
mv.process_move();  // Direct call!
```

## Key Rust Patterns

### 1. **Trait Instead of Virtual Inheritance**

**C++ Problem**: Multiple inheritance (AttackPromotionMove) and diamond problem

**Rust Solution**: Composition with shared data struct
```rust
pub struct AttackPromotionMove {
    shared: SharedMoveData,  // Composition, not inheritance!
}

impl Move for AttackPromotionMove {
    fn process_move(&mut self) {
        // Combines attack + promotion logic
    }
}
```

### 2. **Enum Instead of std::unique_ptr**

**C++ Pattern**:
```cpp
std::vector<std::unique_ptr<Move>> moves;
moves.push_back(std::make_unique<BasicMove>(...));

for (auto& move : moves) {
    move->ProcessMove();  // Virtual dispatch
}
```

**Rust Pattern**:
```rust
let mut moves: Vec<MoveVariant> = vec![];
moves.push(MoveVariant::create(MoveType::BasicMove, ...));

for mv in &mut moves {
    mv.process_move();  // Trait method on enum
}
```

**Benefits**:
- No heap allocation unless needed
- No runtime overhead (monomorphization)
- Exhaustive pattern matching (compiler-checked)
- Simpler ownership semantics

### 3. **Factory Pattern**

**C++ Factory Function**:
```cpp
namespace move_factory {
    std::unique_ptr<Move> CreateMove(const MoveType& type, ...) {
        switch (type) {
            case MoveType::BasicMove:
                return std::make_unique<BasicMove>(...);
            // ...
        }
    }
}
```

**Rust Associated Function**:
```rust
impl MoveVariant {
    pub fn create(move_type: MoveType, ...) -> Self {
        match move_type {
            MoveType::BasicMove => {
                MoveVariant::Basic(BasicMove::new(...))
            }
            // ...
        }
    }
}
```

## Project Structure

```
src/
├── lib.rs           - Main module with comprehensive tests
├── types.rs         - Basic types (Color, PieceType, Position, MoveData)
├── move_trait.rs    - Move trait definition
├── moves.rs         - All 5 move implementations
├── move_variant.rs  - Enum + factory pattern
└── round.rs         - Round and Rounds types

examples/
└── move_demo.rs     - Complete demonstration
```

## All Move Types

### 1. **BasicMove**
Simple piece movement (no capture)
- Example: `e4`, `Nf3`, `Bc4`
- Parses destination and optional source hints

### 2. **AttackMove**
Piece captures another piece
- Example: `exd5`, `Nxe4`, `Bxc3`
- Includes 'x' notation for capture

### 3. **CastlingMove**
King-side or queen-side castling
- Example: `O-O` (kingside), `O-O-O` (queenside)
- Handles both king and rook positions

### 4. **PromotionMove**
Pawn reaches end and promotes
- Example: `e8=Q`, `a1=R`
- Parses promotion piece type

### 5. **AttackPromotionMove**
Pawn captures and promotes
- Example: `exd8=Q`, `fxe1=N`
- Combines attack + promotion logic

## Running Tests

```bash
cd rust/chess_moves
cargo test
```

## Running the Example

```bash
cargo run --example move_demo
```

Output shows:
- Factory pattern creating moves
- Polymorphic move processing
- Trait object usage
- Complete game with rounds
- Move data access

## Usage Examples

### Creating Moves

```rust
use chess_moves::*;

// Using the factory
let mv = MoveVariant::create(
    MoveType::BasicMove,
    Color::White,
    "e4".to_string(),
    "Good opening!".to_string(),
);

// Process the move
mv.process_move();

// Access move data
let data = mv.move_data();
println!("Moving to: ({}, {})", data.to_position.row, data.to_position.col);
```

### Working with Rounds

```rust
let mut game = Rounds::new();

game.push(Round::with_moves(
    1,
    "Opening".to_string(),
    MoveVariant::create(MoveType::BasicMove, Color::White, "e4".to_string(), String::new()),
    MoveVariant::create(MoveType::BasicMove, Color::Black, "e5".to_string(), String::new()),
));

for round in &mut game {
    round.run();
}
```

### Polymorphic Collections

```rust
// Static dispatch (enum)
let moves: Vec<MoveVariant> = vec![
    MoveVariant::create(...),
    MoveVariant::create(...),
];

for mv in &moves {
    println!("{}", mv.move_text());
}

// Dynamic dispatch (trait objects)
let basic = BasicMove::new(...);
let attack = AttackMove::new(...);
let trait_moves: Vec<&dyn Move> = vec![&basic, &attack];

for mv in trait_moves {
    mv.process_move();
}
```

## C++ to Rust Translation Guide

| C++ Feature | Rust Equivalent | Notes |
|-------------|-----------------|-------|
| `class Move { virtual ... }` | `trait Move` | Interface definition |
| `class Derived : public Base` | `struct Derived; impl Trait` | No inheritance |
| `std::unique_ptr<Move>` | `MoveVariant` enum | Owned value |
| `Move*` | `&dyn Move` | Trait object reference |
| `std::vector<unique_ptr<Move>>` | `Vec<MoveVariant>` | Owned collection |
| `virtual void Func()` | `fn func(&mut self)` | Trait method |
| `move->ProcessMove()` | `move.process_move()` | Direct call |
| Multiple inheritance | Composition | Rust doesn't have MI |
| Factory function | Associated function | `Type::create()` |

## Memory and Performance

### C++ (Virtual)
```
sizeof(Move*) = 8 bytes (pointer)
+ vtable lookup per virtual call
+ heap allocation per move
```

### Rust (Enum)
```
sizeof(MoveVariant) = size of largest variant + discriminant
+ no vtable (static dispatch through enum)
+ stack allocated by default
```

### Rust (Trait Object)
```
sizeof(&dyn Move) = 16 bytes (fat pointer: data + vtable)
+ vtable lookup per call (same as C++ virtual)
+ flexible ownership (&, &mut, Box<dyn>)
```

## Key Takeaways

1. **No Virtual Inheritance**: Rust uses traits for polymorphism
2. **No Multiple Inheritance**: Use composition or enum variants
3. **Ownership by Default**: No `unique_ptr` needed - values are moved
4. **Pattern Matching**: Exhaustive `match` replaces `switch`
5. **Zero-Cost Abstractions**: Enum dispatch compiles to efficient code
6. **Memory Safety**: No dangling pointers or use-after-free

## Next Steps

To integrate with the board system:
1. Add actual board state management (instead of println!)
2. Implement move validation using piece rules
3. Add undo/redo functionality
4. Parse PGN format to create moves
5. Integrate with the pieces module

🎯 **Complete move system with all 5 move types + factory + rounds!**
