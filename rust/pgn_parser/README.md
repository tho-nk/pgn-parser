# PGN Parser - Rust Implementation

This module ports the C++ PGN (Portable Game Notation) parser from the chess project to idiomatic Rust.

## Source Files

**C++ Implementation:**
- `src/chess/common/include/ParsingHelper.hpp` - Header with function declarations
- `src/chess/common/src/ParsingHelper.cpp` - Implementation

**Rust Implementation:**
- `src/parsing.rs` - Core parsing functions
- `src/lib.rs` - Public API and re-exports
- `examples/parse_demo.rs` - Usage demonstration

## PGN Format Overview

PGN files contain chess games in a standardized text format:

```pgn
[Event "World Championship"]
[Site "Moscow"]
[Date "1985.09.10"]
[Round "1"]
[White "Kasparov, Garry"]
[Black "Karpov, Anatoly"]
[Result "1-0"]

1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 {This is the Ruy Lopez}
4. Ba4 Nf6 5. O-O (5. d3 is also popular) Be7 6. Re1 b5
...
42. Rxd7 1-0
```

### Key Elements

- **Tag Pairs**: `[TagName "Value"]` - Metadata about the game
- **Move Text**: `1. e4 e5` - Actual game moves
- **Comments**: `{text comment}` - Annotations in braces
- **Variations**: `(alternative move)` - Alternative lines in parentheses
- **NAG**: `$1`, `$2` - Numeric Annotation Glyphs
- **Result**: `1-0`, `0-1`, `1/2-1/2`, `*` - Game outcome

## C++ to Rust Translation

### Type Mappings

| C++ Type | Rust Type | Notes |
|----------|-----------|-------|
| `std::string_view` | `&str` | Borrowed string slice |
| `std::string` | `String` | Owned string |
| `std::queue<std::string>` | `VecDeque<String>` | Double-ended queue |
| `std::ifstream` | `BufReader<File>` | Buffered file reader |
| `std::filesystem::path` | `std::path::Path` | File system path |
| `size_t` | `usize` | Unsigned size |

### Function Mappings

| C++ Function | Rust Function | Description |
|--------------|---------------|-------------|
| `helper::IsBalanced(sv)` | `is_balanced(&str)` | Check bracket matching |
| `helper::GetNextNonSpace(sv, pos)` | `get_next_non_space(&str, usize)` | Find next non-whitespace |
| `helper::GetNextSpace(sv, pos)` | `get_next_space(&str, usize)` | Find next whitespace |
| `helper::GetComment(sv, out, pos)` | `get_comment(&str, &mut usize)` | Extract comments/variations |
| `helper::LeftTrim(s)` | `left_trim(&mut String)` | Trim left whitespace |
| `helper::RightTrim(s)` | `right_trim(&mut String)` | Trim right whitespace |
| `helper::TrimSpace(s)` | `trim_space(&mut String)` | Trim both sides |
| `helper::removeUnwantedChars(s)` | `remove_unwanted_chars(&mut String)` | Remove +, #, !, ? |
| `helper::Remove3Dot(s)` | `remove_3_dot(&mut String)` | Remove `...` prefix |
| `helper::ProcessGameComment(s)` | `process_game_comment(&mut String)` | Extract first round |
| `helper::ParseFile(path)` | `parse_file(&Path)` | Parse entire file |

### Struct Mapping

**C++ ParsingHelper:**
```cpp
struct ParsingHelper {
    std::queue<std::string> roundQueue;
    std::string lastRun;
};
```

**Rust ParsingResult:**
```rust
pub struct ParsingResult {
    pub round_queue: VecDeque<String>,
    pub last_run: String,
}
```

## Key Design Differences

### 1. String Handling

**C++:** Uses `std::string_view` for non-owning references, `std::string&` for mutable references
```cpp
bool IsBalanced(std::string_view text);
void TrimSpace(std::string& s);
```

**Rust:** Uses `&str` for borrowed slices, `&mut String` for mutable references
```rust
fn is_balanced(text: &str) -> bool;
fn trim_space(s: &mut String);
```

### 2. File I/O

**C++:** Uses `std::ifstream` with `getline()`
```cpp
std::ifstream file(path);
std::string line;
while (std::getline(file, line)) {
    // process line
}
```

**Rust:** Uses `BufReader` with iterator
```rust
let file = File::open(path)?;
let reader = BufReader::new(file);
for line_result in reader.lines() {
    let line = line_result?;
    // process line
}
```

### 3. Collections

**C++:** Uses `std::queue` (adapter over `std::deque`)
```cpp
std::queue<std::string> roundQueue;
roundQueue.push(round);
```

**Rust:** Uses `VecDeque` directly
```rust
let mut round_queue = VecDeque::new();
round_queue.push_back(round);
```

### 4. Error Handling

**C++:** No explicit error handling in original (assumes file exists)
```cpp
ParsingHelper ParseFile(const std::filesystem::path& path);
```

**Rust:** Returns `Result` for I/O operations
```rust
fn parse_file(path: &Path) -> std::io::Result<ParsingResult>;
```

## Usage Examples

### Basic Parsing

```rust
use pgn_parser::parse_file;
use std::path::Path;

let result = parse_file(Path::new("game.pgn"))?;

println!("Found {} rounds", result.round_queue.len());
for round in result.round_queue {
    println!("{}", round);
}
println!("Result: {}", result.last_run);
```

### String Utilities

```rust
use pgn_parser::{is_balanced, remove_unwanted_chars, trim_space};

// Check bracket balance
assert!(is_balanced("(variation) {comment}"));
assert!(!is_balanced("(unbalanced"));

// Clean up move notation
let mut move_str = "Nf3+".to_string();
remove_unwanted_chars(&mut move_str);
assert_eq!(move_str, "Nf3");

// Trim whitespace
let mut text = "  hello  ".to_string();
trim_space(&mut text);
assert_eq!(text, "hello");
```

### Parsing Text Directly

```rust
use pgn_parser::parse_text;

let pgn = r#"
[Event "Test"]
1. e4 e5 2. Nf3 Nc6 3. Bb5
"#;

let result = parse_text(pgn);
assert_eq!(result.round_queue.len(), 2);
```

## Building and Testing

```bash
# Build the library
cargo build

# Run tests
cargo test

# Run the demo example
cargo run --example parse_demo

# Build with optimizations
cargo build --release
```

## Implementation Notes

### Bracket Balancing

The `is_balanced()` function tracks nesting depth for parentheses `()` and braces `{}`:
- Increments counter on opening bracket
- Decrements on closing bracket
- Returns `false` if counter goes negative (means closing before opening)
- Returns `true` only if both counters are zero at the end

### Round Splitting

The `parse_file()` function splits moves by round numbers:
1. Reads file line by line, skipping metadata tags `[...]`
2. Looks for round markers: `"2."`, `"3."`, etc.
3. Only splits at a round marker if the text before it is balanced
4. Special handling for first round (round 2) to remove game comments

### Comment Extraction

The `get_comment()` function handles nested structures:
- Variations in parentheses: `(5. d3 is also good)`
- Comments in braces: `{This is the Ruy Lopez}`
- NAGs (Numeric Annotation Glyphs): `$1`, `$2`, etc.

## Performance Considerations

- **String allocation**: Functions like `trim_space()` create new strings. Consider using string slices where possible for read-only operations.
- **Iterator efficiency**: File parsing uses line-by-line iteration with `BufReader`, which is efficient for large files.
- **Early returns**: `is_balanced()` returns early if counters go negative, avoiding unnecessary processing.

## Future Enhancements

1. **Streaming parser**: Process PGN files without loading entire game into memory
2. **Parallel parsing**: Use `rayon` to parse multiple games concurrently
3. **Validation**: Add strict PGN format validation with detailed error messages
4. **AST representation**: Parse move text into structured Abstract Syntax Tree
5. **Integration**: Connect with `chess_moves` module to create `MoveVariant` instances

## References

- [PGN Specification](https://www.chessclub.com/help/PGN-spec)
- [Standard: Portable Game Notation](https://github.com/mliebelt/pgn-spec)
