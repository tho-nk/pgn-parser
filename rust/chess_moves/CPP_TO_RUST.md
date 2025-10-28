# C++ Move System → Rust: Complete Translation

## Overview

This document shows **exactly** how the C++ move/inheritance system translates to idiomatic Rust.

---

## The C++ Inheritance Hierarchy

```
                    Move (virtual base)
                       │
            ┌──────────┼──────────┬─────────────┐
            │          │          │             │
       BasicMove   AttackMove  CastlingMove  PromotionMove
                      (virtual)               (virtual)
                         │                       │
                         └───────────┬───────────┘
                                     │
                            AttackPromotionMove
                          (multiple inheritance)
```

### C++ Code Structure

```cpp
// Base class with virtual functions
class Move {
protected:
    MoveType moveType_;
    MoveData moveData_;
public:
    virtual void ProcessMove() = 0;
    virtual ~Move() = default;
};

// Simple inheritance
class BasicMove : public Move {
    void ProcessMove() final override;
};

// Virtual inheritance (for diamond problem)
class AttackMove : public virtual Move {
    void ProcessMove() override;
};

class PromotionMove : public virtual Move {
    void ProcessMove() override;
};

// Multiple inheritance
class AttackPromotionMove : public AttackMove, public PromotionMove {
    void ProcessMove() final override;
};

// Factory with unique_ptr
std::unique_ptr<Move> CreateMove(MoveType type, ...) {
    switch (type) {
        case MoveType::BasicMove:
            return std::make_unique<BasicMove>(...);
        // ...
    }
}

// Usage
std::vector<std::unique_ptr<Move>> moves;
for (auto& move : moves) {
    move->ProcessMove();  // Virtual dispatch through vtable
}
```

---

## The Rust Equivalent

```
                    trait Move
                       │
            ┌──────────┼──────────┬─────────────┐
            │          │          │             │
       BasicMove   AttackMove  CastlingMove  PromotionMove
       (struct)     (struct)     (struct)      (struct)
            │          │          │             │
            └──────────┴──────────┴─────────────┘
                         │
                    MoveVariant (enum)
            (wraps all types, implements Move)
```

### Rust Code Structure

```rust
// Trait (interface)
pub trait Move {
    fn process_move(&mut self);
    fn move_data(&self) -> &MoveData;
}

// Shared data (replaces protected members)
pub struct SharedMoveData {
    move_type: MoveType,
    move_data: MoveData,
}

// Concrete types (no inheritance!)
pub struct BasicMove {
    shared: SharedMoveData,
}

impl Move for BasicMove {
    fn process_move(&mut self) { /* implementation */ }
}

pub struct AttackMove {
    shared: SharedMoveData,
}

impl Move for AttackMove {
    fn process_move(&mut self) { /* implementation */ }
}

// For "multiple inheritance" case, just compose logic
pub struct AttackPromotionMove {
    shared: SharedMoveData,
}

impl Move for AttackPromotionMove {
    fn process_move(&mut self) {
        // Combines attack + promotion logic directly
    }
}

// Enum for polymorphism (replaces unique_ptr<Move>)
pub enum MoveVariant {
    Basic(BasicMove),
    Attack(AttackMove),
    Castling(CastlingMove),
    Promotion(PromotionMove),
    AttackPromotion(AttackPromotionMove),
}

// Implement trait on the enum!
impl Move for MoveVariant {
    fn process_move(&mut self) {
        match self {
            MoveVariant::Basic(m) => m.process_move(),
            MoveVariant::Attack(m) => m.process_move(),
            // ... delegate to inner type
        }
    }
}

// Factory (associated function)
impl MoveVariant {
    pub fn create(move_type: MoveType, ...) -> Self {
        match move_type {
            MoveType::BasicMove => MoveVariant::Basic(BasicMove::new(...)),
            // ...
        }
    }
}

// Usage - NO unique_ptr needed!
let mut moves: Vec<MoveVariant> = vec![];
for mv in &mut moves {
    mv.process_move();  // Direct call, no vtable!
}
```

---

## Side-by-Side: AttackPromotionMove

The most complex case - multiple inheritance in C++.

### C++ (Multiple Inheritance)

```cpp
class AttackPromotionMove : public AttackMove, public PromotionMove {
public:
    AttackPromotionMove(...) : Move(...), AttackMove(), PromotionMove() {}
    
    void ProcessMove() final override {
        // Must carefully call base class methods
        // Virtual inheritance avoids diamond problem
    }
};
```

**Problems:**
- Diamond inheritance problem (solved with `virtual` keyword)
- Complex initialization order
- Ambiguity in member access
- Vtable overhead for multiple bases

### Rust (Composition)

```rust
pub struct AttackPromotionMove {
    shared: SharedMoveData,  // Just one data structure!
}

impl AttackPromotionMove {
    pub fn new(...) -> Self {
        let mut move = Self {
            shared: SharedMoveData::new(...),
        };
        move.compute_move_data();
        move
    }
    
    fn compute_move_data(&mut self) {
        // Parse "exd8=Q" format
        // Combines attack parsing + promotion parsing
        // No base class calls - just implement the logic!
    }
}

impl Move for AttackPromotionMove {
    fn process_move(&mut self) {
        // Implementation combines attack + promotion behavior
        println!("Attack and promote: {}", self.shared.move_text);
    }
}
```

**Benefits:**
- No inheritance complexity
- Clear data ownership
- Single implementation site
- No vtable

---

## Memory Layout Comparison

### C++ Virtual Inheritance

```
AttackPromotionMove object:
┌─────────────┐
│ vtable ptr  │ (8 bytes) → AttackMove vtable
├─────────────┤
│ vtable ptr  │ (8 bytes) → PromotionMove vtable
├─────────────┤
│ vtable ptr  │ (8 bytes) → Move vtable
├─────────────┤
│ moveType_   │ (4 bytes)
├─────────────┤
│ moveData_   │ (N bytes)
└─────────────┘
Total: ~24+ bytes + data
```

### Rust Enum

```
MoveVariant object:
┌─────────────┐
│ discriminant│ (1 byte) → which variant
├─────────────┤
│ variant data│ (size of largest variant)
│             │
│ shared:     │
│   move_type │
│   move_data │
└─────────────┘
Total: 1 + sizeof(largest variant)
No vtables, no pointers!
```

---

## Factory Pattern Comparison

### C++ Factory

```cpp
namespace move_factory {
    std::unique_ptr<Move> CreateMove(
        const MoveType &moveType,
        const Color &color,
        std::string &&moveText,
        std::string &&comment
    ) {
        switch (moveType) {
        case MoveType::BasicMove:
            return std::make_unique<BasicMove>(
                moveType, color,
                std::move(moveText),
                std::move(comment)
            );
        case MoveType::AttackMove:
            return std::make_unique<AttackMove>(
                moveType, color,
                std::move(moveText),
                std::move(comment)
            );
        // ... more cases
        }
    }
}

// Usage
auto move = move_factory::CreateMove(
    MoveType::BasicMove,
    Color::White,
    "e4",
    "Good move"
);
move->ProcessMove();
```

### Rust Factory

```rust
impl MoveVariant {
    pub fn create(
        move_type: MoveType,
        color: Color,
        move_text: String,
        comment: String,
    ) -> Self {
        match move_type {
            MoveType::BasicMove => {
                MoveVariant::Basic(BasicMove::new(
                    move_type,
                    color,
                    move_text,
                    comment,
                ))
            }
            MoveType::AttackMove => {
                MoveVariant::Attack(AttackMove::new(
                    move_type,
                    color,
                    move_text,
                    comment,
                ))
            }
            // ... more cases
        }
    }
}

// Usage - no pointer!
let mv = MoveVariant::create(
    MoveType::BasicMove,
    Color::White,
    "e4".to_string(),
    "Good move".to_string(),
);
mv.process_move();
```

---

## Collection Management

### C++ with unique_ptr

```cpp
std::vector<std::unique_ptr<Move>> moves;

// Adding moves
moves.push_back(move_factory::CreateMove(...));
moves.push_back(std::make_unique<BasicMove>(...));

// Iterating
for (const auto& move : moves) {
    move->ProcessMove();  // Virtual dispatch
    auto type = move->GetMoveType();
}

// Moving ownership
auto move = std::move(moves[0]);
moves.erase(moves.begin());
```

### Rust with Enum

```rust
let mut moves: Vec<MoveVariant> = vec![];

// Adding moves
moves.push(MoveVariant::create(...));
moves.push(MoveVariant::Basic(BasicMove::new(...)));

// Iterating
for mv in &moves {
    mv.process_move();  // Direct call
    let move_type = mv.move_type();
}

// Moving ownership
let mv = moves.remove(0);  // Moves out of vector
```

---

## Performance Characteristics

| Operation | C++ (virtual) | Rust (enum) | Rust (trait object) |
|-----------|---------------|-------------|---------------------|
| **Call overhead** | 1 vtable lookup | 0 (direct) | 1 vtable lookup |
| **Memory per object** | 8-24 bytes overhead | 1 byte tag | 16 bytes (fat ptr) |
| **Heap allocation** | Always (unique_ptr) | Optional | Optional |
| **Inlining** | Difficult | Easy | Difficult |
| **Cache locality** | Poor (pointer chase) | Excellent | Poor |

---

## When to Use Each Rust Pattern

### Use `enum` (MoveVariant) when:
- ✅ You have a closed set of types (5 move types)
- ✅ You want zero-cost abstraction
- ✅ You control all the implementations
- ✅ **This is your case!**

### Use `Box<dyn Trait>` when:
- You need dynamic dispatch
- Open set of types (plugins, extensions)
- Heterogeneous collections of unknown types
- Interfacing with C (FFI)

### Use generics `<T: Trait>` when:
- Static dispatch across different types
- Library code (let caller choose type)
- Maximum performance critical code

---

## Summary

| Feature | C++ Approach | Rust Equivalent |
|---------|-------------|-----------------|
| Polymorphism | Virtual functions | Traits |
| Inheritance | `class A : public B` | `impl Trait for Type` |
| Multiple Inheritance | Diamond problem | Composition |
| Dynamic Dispatch | `Base*` + vtable | `&dyn Trait` |
| Factory | `unique_ptr<Base>` | `enum` variants |
| Memory | Heap allocated | Stack by default |
| Ownership | `unique_ptr` explicit | Automatic (move) |

**The Rust way is simpler, safer, and often faster!** 🚀
