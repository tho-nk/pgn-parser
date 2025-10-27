# Rust Pieces - Chess Piece Implementation

This is a **complete Rust port** of the C++ chess piece implementation from `src/chess/piece/`. 

**All 6 chess piece types are implemented**: Rook, Knight, Bishop, Queen, King, and Pawn.

## What's Included

✅ **All Chess Pieces**: Rook, Knight, Bishop, Queen, King, Pawn, EmptyPiece  
✅ **Trait-based Architecture**: Clean separation of data and behavior  
✅ **Enum Variants**: Zero-cost abstraction with static dispatch  
✅ **Comprehensive Tests**: Movement validation for all piece types  
✅ **Working Example**: `examples/all_pieces.rs` demonstrates all patterns

## Architecture Overview

### C++ to Rust Pattern Mapping

#### 1. **CRTP Pattern → Rust Traits + Generics**

**C++ (CRTP):**
```cpp
template <typename Derived>
class BaseCRTP {
    Derived& self() { return *static_cast<Derived*>(this); }
};

class Rook : public BaseCRTP<Rook> {
    // implementation
};
```

**Rust (Traits):**
```rust
trait Piece {
    fn is_valid_basic_move(&self, to: Position) -> bool;
}

struct Rook { /* fields */ }

impl Piece for Rook {
    fn is_valid_basic_move(&self, to: Position) -> bool {
        // implementation
    }
}
```

#### 2. **Virtual Inheritance → Trait Objects**

**C++ (Virtual):**
```cpp
class BasePiece {
public:
    virtual bool IsValidMove() const = 0;
};

std::vector<BasePiece*> pieces;
```

**Rust (Trait Objects):**
```rust
trait Piece {
    fn is_valid_move(&self) -> bool;
}

let pieces: Vec<Box<dyn Piece>> = vec![/* ... */];
```

#### 3. **std::variant → Rust Enum**

**C++ (variant):**
```cpp
using Piece = std::variant<EmptyPiece, Rook, Knight, Bishop>;

std::visit([](auto& p) {
    return p.Process();
}, piece);
```

**Rust (enum + match):**
```rust
enum PieceVariant {
    Empty(EmptyPiece),
    Rook(Rook),
    Knight(Knight),
    Bishop(Bishop),
}

match piece {
    PieceVariant::Rook(r) => r.process(),
    // ...
}
```

#### 4. **Template Functions → Generic Functions with Closures**

**C++ (template parameter):**
```cpp
template <typename IsEmpty>
bool ValidateMove_(const IsEmpty& isEmpty) {
    if (!isEmpty(p)) { /* ... */ }
}
```

**Rust (generic with Fn trait):**
```rust
fn validate_move<F>(is_empty: F) -> bool
where
    F: Fn(Position) -> bool,
{
    if !is_empty(p) { /* ... */ }
}
```

## Key Rust Concepts for C++ Developers

### 1. Ownership & Borrowing
- Rust has no garbage collector or raw pointers (by default)
- Every value has a single owner
- Use `&T` for immutable borrow, `&mut T` for mutable borrow
- Smart pointers: `Box<T>` (like `unique_ptr`), `Rc<T>` (like `shared_ptr`)

### 2. No Inheritance, Use Composition
- Rust doesn't have class inheritance
- Use struct composition + traits for polymorphism
- `BasePieceData` is a struct field, not a base class

### 3. Enums are Powerful
- Rust enums are sum types (algebraic data types)
- Can hold data in variants
- Replace `std::variant` and many inheritance patterns

### 4. Pattern Matching
- `match` is exhaustive (compiler checks all cases)
- Replace if/else chains and switch statements
- Works with enums, tuples, structs, and more

### 5. Traits vs Interfaces
- Traits are like C++ concepts + interfaces
- Can have default implementations
- Can be generic over types

## Project Structure

```
src/
├── lib.rs           - Main module with comprehensive tests
├── types.rs         - Basic types (Color, PieceType, Position)
├── base_piece.rs    - BasePieceData and Piece trait
├── pieces.rs        - All 6 pieces: Rook, Knight, Bishop, Queen, King, Pawn
└── variant.rs       - PieceVariant enum and Board type

examples/
└── all_pieces.rs    - Complete demonstration of all features
```

## All Implemented Pieces

### 1. **Rook** 🏰
- Moves horizontally or vertically
- Can move any number of squares
- Cannot jump over pieces

### 2. **Knight** 🐴
- Moves in L-shape (2+1 squares)
- Can jump over other pieces
- 8 possible moves from any position

### 3. **Bishop** 🎩
- Moves diagonally
- Can move any number of squares
- Cannot jump over pieces

### 4. **Queen** 👑
- Most powerful piece
- Combines Rook + Bishop movement
- Can move horizontally, vertically, or diagonally

### 5. **King** 🤴
- Moves one square in any direction
- 8 possible moves (adjacent squares)
- Most important piece to protect

### 6. **Pawn** ⚔️
- Most complex movement rules
- Moves forward 1 square (2 from start)
- Attacks diagonally
- Supports en passant logic

## Running Tests

```bash
cd rust/pieces
cargo test
```

## Running the Complete Example

```bash
cd rust/pieces
cargo run --example all_pieces
```

This example demonstrates:
- Creating all 6 piece types
- Using the trait interface
- Enum-based polymorphism
- Board representation
- Move validation

## Running with Output

```bash
cargo test -- --nocapture
```

## Building

```bash
cargo build          # Debug build
cargo build --release # Optimized build
```

## Performance Notes

1. **Static Dispatch (enum)**: Zero-cost abstraction, as fast as C++ CRTP
   - Use `PieceVariant` with `match`
   - Compiler knows exact type at compile time

2. **Dynamic Dispatch (trait objects)**: Small runtime cost, like C++ virtual
   - Use `Box<dyn Piece>` or `&dyn Piece`
   - One vtable lookup per call

3. **Monomorphization**: Rust generics create specialized code per type
   - Like C++ templates
   - No runtime cost, but increases binary size

## Next Steps for Learning Rust

1. **Ownership Deep Dive**: Read [The Rust Book Chapter 4](https://doc.rust-lang.org/book/ch04-00-understanding-ownership.html)
2. **Traits**: [Chapter 10](https://doc.rust-lang.org/book/ch10-02-traits.html)
3. **Enums & Pattern Matching**: [Chapter 6](https://doc.rust-lang.org/book/ch06-00-enums.html)
4. **Error Handling**: [Chapter 9](https://doc.rust-lang.org/book/ch09-00-error-handling.html) - Result and Option types

## Porting More C++ Code

All pieces are now implemented! The architecture supports easy extension:

### To Add Custom Behavior

1. **Add methods to the `Piece` trait** in `base_piece.rs`
2. **Implement for all types** in `pieces.rs`
3. **Update PieceVariant** match statements in `variant.rs`
4. **Add tests** in `lib.rs`

### To Add Special Moves (Castling, En Passant)

1. Add state to `BasePieceData` or individual piece structs
2. Implement special move logic in piece-specific methods
3. Add board-level validation in a separate module

### Example: Adding a Method

```rust
// In base_piece.rs
pub trait Piece {
    // ... existing methods ...
    
    fn can_castle(&self) -> bool {
        false  // default implementation
    }
}

// In pieces.rs - King implementation
impl Piece for King {
    fn can_castle(&self) -> bool {
        // King-specific castling logic
        true
    }
}
```
