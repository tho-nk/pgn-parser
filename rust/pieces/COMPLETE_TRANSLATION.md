# Complete C++ to Rust Translation - All Chess Pieces

This document shows the **complete translation** of all 6 chess piece types from C++ to Rust.

## Overview

| Piece   | C++ Class | Rust Struct | Move Pattern |
|---------|-----------|-------------|--------------|
| Rook    | ✅ `Rook`   | ✅ `Rook`     | Horizontal/Vertical |
| Knight  | ✅ `Knight` | ✅ `Knight`   | L-shape (2+1) |
| Bishop  | ✅ `Bishop` | ✅ `Bishop`   | Diagonal |
| Queen   | ✅ `Queen`  | ✅ `Queen`    | Rook + Bishop |
| King    | ✅ `King`   | ✅ `King`     | One square any direction |
| Pawn    | ✅ `Pawn`   | ✅ `Pawn`     | Forward 1-2, attack diagonal |

---

## 1. Rook (Horizontal/Vertical)

### C++ Implementation
```cpp
class Rook : public BasePiece {
public:
    Rook(const Color &color, const Position &position);
private:
    bool IsValidBasicMove_(const Position &toPosition, ...) const;
    bool IsValidAttackMove_(const Position &toPosition, ...) const;
    bool IsValidMove_(const Position &toPosition, ...) const;
};

bool Rook::IsValidMove_(...) const {
    int dRow = toPosition.row - GetPosition().row;
    int dCol = toPosition.col - GetPosition().col;
    
    if (dRow != 0 && dCol != 0) { return false; }
    
    return ValidateMove_(dRow, dCol, toPosition, ...);
}
```

### Rust Implementation
```rust
pub struct Rook {
    base: BasePieceData,
}

impl Piece for Rook {
    fn is_valid_basic_move_impl(&self, to_position: Position, ...) -> bool {
        let d_row = to_position.row - self.base.position.row;
        let d_col = to_position.col - self.base.position.col;
        
        if d_row != 0 && d_col != 0 { return false; }
        
        validate_move(d_row, d_col, to_position, ...)
    }
}
```

---

## 2. Knight (L-shaped Jump)

### C++ Implementation
```cpp
bool Knight::IsValidMove_(const Position &toPosition) const {
    constexpr int dr[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    constexpr int dc[] = {-1, -2, -2, -1, 1, 2, 2, 1};
    
    for (int index = 0; index < 8; ++index) {
        Position p{GetPosition().row + dr[index], 
                   GetPosition().col + dc[index]};
        if (p == toPosition) { return true; }
    }
    return false;
}
```

### Rust Implementation
```rust
fn is_valid_move(&self, to_position: Position) -> bool {
    const DR: [i32; 8] = [-2, -1, 1, 2, 2, 1, -1, -2];
    const DC: [i32; 8] = [-1, -2, -2, -1, 1, 2, 2, 1];
    
    for i in 0..8 {
        let p = Position::new(
            self.base.position.row + DR[i],
            self.base.position.col + DC[i],
        );
        if p == to_position { return true; }
    }
    false
}
```

**Key Differences:**
- Rust: `const` arrays (compile-time)
- Rust: explicit `.new()` constructor
- Rust: for-in range syntax `0..8`

---

## 3. Bishop (Diagonal)

### C++ Implementation
```cpp
bool Bishop::IsValidMove_(...) const {
    int dRow = toPosition.row - GetPosition().row;
    int dCol = toPosition.col - GetPosition().col;
    
    // Bishop moves only diagonally
    if (std::abs(dRow) != std::abs(dCol)) {
        return false;
    }
    
    return ValidateMove_(dRow, dCol, toPosition, ...);
}
```

### Rust Implementation
```rust
fn is_valid_move_with_check<F>(&self, to_position: Position, ..., is_empty: F) -> bool
where
    F: Fn(Position) -> bool,
{
    let d_row = to_position.row - self.base.position.row;
    let d_col = to_position.col - self.base.position.col;
    
    if d_row.abs() != d_col.abs() {
        return false;
    }
    
    validate_move(d_row, d_col, to_position, ..., is_empty)
}
```

---

## 4. Queen (Rook + Bishop)

### C++ Implementation
```cpp
bool Queen::IsValidMove_(...) const {
    int dRow = toPosition.row - GetPosition().row;
    int dCol = toPosition.col - GetPosition().col;
    
    // Queen: horizontal, vertical, OR diagonal
    if ((dRow != 0 && dCol != 0) && 
        (std::abs(dRow) != std::abs(dCol))) {
        return false;
    }
    
    return ValidateMove_(dRow, dCol, toPosition, ...);
}
```

### Rust Implementation
```rust
fn is_valid_move_with_check<F>(&self, to_position: Position, ..., is_empty: F) -> bool
where
    F: Fn(Position) -> bool,
{
    let d_row = to_position.row - self.base.position.row;
    let d_col = to_position.col - self.base.position.col;
    
    // Queen moves horizontally, vertically, or diagonally
    if (d_row != 0 && d_col != 0) && (d_row.abs() != d_col.abs()) {
        return false;
    }
    
    validate_move(d_row, d_col, to_position, ..., is_empty)
}
```

---

## 5. King (One Square Any Direction)

### C++ Implementation
```cpp
bool King::IsValidMove_(const Position &toPosition) const {
    constexpr int dr[] = {-1, 0, 1, 1, 1, 0, -1, -1};
    constexpr int dc[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    
    for (int index = 0; index < 8; ++index) {
        Position p{GetPosition().row + dr[index], 
                   GetPosition().col + dc[index]};
        if (p == toPosition) { return true; }
    }
    return false;
}
```

### Rust Implementation
```rust
fn is_valid_move(&self, to_position: Position) -> bool {
    const DR: [i32; 8] = [-1, 0, 1, 1, 1, 0, -1, -1];
    const DC: [i32; 8] = [-1, -1, -1, 0, 1, 1, 1, 0];
    
    for i in 0..8 {
        let p = Position::new(
            self.base.position.row + DR[i],
            self.base.position.col + DC[i],
        );
        if p == to_position { return true; }
    }
    false
}
```

**Pattern:** Same as Knight, but different offsets (adjacent squares instead of L-shape).

---

## 6. Pawn (Complex Movement)

### C++ Implementation
```cpp
bool Pawn::IsValidBasicMove_(...) const {
    int dRow = toPosition.row - GetPosition().row;
    int dCol = toPosition.col - GetPosition().col;
    
    if (GetColor() == Color::White) {
        if (dCol == 0) {
            // One step forward
            if (dRow == 1 && Square::GetInstance().IsEmptyAt(toPosition)) {
                return true;
            }
            // Two steps from initial position
            if (dRow == 2 && GetPosition().row == 1 && ...) {
                return true;
            }
        }
    } else if (GetColor() == Color::Black) {
        // Black pawns move down (negative dRow)
        if (dCol == 0) {
            if (dRow == -1 && ...) { return true; }
            if (dRow == -2 && GetPosition().row == 6 && ...) { return true; }
        }
    }
    return false;
}
```

### Rust Implementation
```rust
fn is_valid_basic_move_with_check<F>(&self, to_position: Position, is_empty: F) -> bool
where
    F: Fn(Position) -> bool,
{
    let d_row = to_position.row - self.base.position.row;
    let d_col = to_position.col - self.base.position.col;
    
    match self.base.color {
        Color::White => {
            if d_col == 0 {
                // One step forward
                if d_row == 1 && is_empty(to_position) {
                    return true;
                }
                // Two steps from initial position (row 1)
                if d_row == 2 && self.base.position.row == 1 && ... {
                    return true;
                }
            }
        }
        Color::Black => {
            if d_col == 0 {
                if d_row == -1 && is_empty(to_position) { return true; }
                if d_row == -2 && self.base.position.row == 6 && ... { return true; }
            }
        }
        Color::Undefined => return false,
    }
    false
}
```

**Rust Pattern Matching:** `match` replaces C++ `if-else` chains and is exhaustive (compiler enforces all cases).

---

## Polymorphism Comparison

### C++ std::variant Approach
```cpp
using Piece = std::variant<EmptyPiece, Bishop, King, Knight, Pawn, Queen, Rook>;

std::vector<Piece> pieces;
pieces.emplace_back(Rook{...});
pieces.emplace_back(Queen{...});

for (auto& piece : pieces) {
    std::visit([](auto& p) {
        auto result = p.IsValidBasicMove(...);
    }, piece);
}
```

### Rust Enum Approach
```rust
pub enum PieceVariant {
    Empty(EmptyPiece),
    Bishop(Bishop),
    King(King),
    Knight(Knight),
    Pawn(Pawn),
    Queen(Queen),
    Rook(Rook),
}

impl Piece for PieceVariant {
    fn is_valid_basic_move_impl(&self, ...) -> bool {
        match self {
            PieceVariant::Rook(p) => p.is_valid_basic_move_impl(...),
            PieceVariant::Queen(p) => p.is_valid_basic_move_impl(...),
            // ... all variants
        }
    }
}

let pieces: Vec<PieceVariant> = vec![
    PieceVariant::Rook(Rook::new(...)),
    PieceVariant::Queen(Queen::new(...)),
];

for piece in &pieces {
    let result = piece.is_valid_basic_move(...); // Direct call!
}
```

**Key Advantage:** In Rust, `PieceVariant` implements `Piece` directly, so no `visit` needed!

---

## Memory Layout Comparison

### C++ (CRTP)
```
BaseCRTP<Rook>
└─ Rook
   └─ type_, color_, position_
```
- Compile-time polymorphism
- No vtable
- Zero runtime cost

### C++ (Virtual)
```
BasePtr*  →  [vtable ptr | type_ | color_ | position_]
```
- Runtime polymorphism
- Virtual table lookup
- Small overhead per call

### Rust (Enum)
```
PieceVariant (discriminant + union of all variants)
├─ tag: u8 (which variant)
└─ data: largest variant size
```
- Compile-time known size
- Match on tag (branch prediction friendly)
- Zero-cost abstraction

### Rust (Trait Object)
```
&dyn Piece  →  [data ptr | vtable ptr]
```
- Runtime polymorphism
- Virtual table lookup
- Same cost as C++ virtual

---

## Summary

| Feature | C++ | Rust |
|---------|-----|------|
| **Inheritance** | `class Rook : public BasePiece` | `struct Rook` + `impl Piece for Rook` |
| **CRTP** | `BaseCRTP<Derived>` | Traits with generics |
| **Virtual** | `virtual` keyword + vtable | `&dyn Trait` + vtable |
| **Variant** | `std::variant` + `std::visit` | `enum` + `match` |
| **Templates** | `template<typename T>` | `fn foo<T: Trait>()` or closures |
| **Memory** | Manual `new`/`delete` | Automatic ownership/borrowing |

**All 6 pieces fully implemented in Rust!** 🎉
