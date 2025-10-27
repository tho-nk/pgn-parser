use pieces::*;

fn main() {
    println!("=== Rust Chess Pieces - Complete Implementation ===\n");

    // Create all 6 chess piece types
    let rook = Rook::new(Color::White, Position::new(0, 0));
    let knight = Knight::new(Color::White, Position::new(0, 1));
    let bishop = Bishop::new(Color::White, Position::new(0, 2));
    let queen = Queen::new(Color::White, Position::new(0, 3));
    let king = King::new(Color::White, Position::new(0, 4));
    let pawn = Pawn::new(Color::White, Position::new(1, 0));

    println!("1. Direct piece usage (concrete types):");
    println!("   Rook at (0,0) can move to (0,7)? {}", 
        rook.is_valid_basic_move(Position::new(0, 7), None));
    println!("   Knight at (0,1) can move to (2,2)? {}", 
        knight.is_valid_basic_move(Position::new(2, 2), None));
    println!("   Bishop at (0,2) can move to (3,5)? {}", 
        bishop.is_valid_basic_move(Position::new(3, 5), None));
    println!("   Queen at (0,3) can move to (7,3)? {}", 
        queen.is_valid_basic_move(Position::new(7, 3), None));
    println!("   King at (0,4) can move to (1,4)? {}", 
        king.is_valid_basic_move(Position::new(1, 4), None));
    println!("   Pawn at (1,0) can move to (2,0)? {}", 
        pawn.is_valid_basic_move(Position::new(2, 0), None));

    println!("\n2. Using PieceVariant enum (like C++ std::variant):");
    
    // Create a heterogeneous collection using the enum
    let pieces: Vec<PieceVariant> = vec![
        PieceVariant::Rook(Rook::new(Color::White, Position::new(0, 0))),
        PieceVariant::Knight(Knight::new(Color::Black, Position::new(7, 6))),
        PieceVariant::Bishop(Bishop::new(Color::White, Position::new(2, 2))),
        PieceVariant::Queen(Queen::new(Color::Black, Position::new(7, 3))),
        PieceVariant::King(King::new(Color::White, Position::new(4, 4))),
        PieceVariant::Pawn(Pawn::new(Color::Black, Position::new(6, 5))),
    ];

    // Call trait methods directly on the enum - zero-cost abstraction!
    for piece in &pieces {
        let base = piece.base();
        println!("   {} {} at ({},{}) - type: {:?}",
            base.color, base.piece_type,
            base.position.row, base.position.col,
            std::mem::discriminant(piece)
        );
    }

    println!("\n3. Dynamic dispatch with trait objects:");
    
    // Use trait objects for runtime polymorphism (like C++ virtual)
    let dynamic_pieces: Vec<&dyn Piece> = vec![
        &rook, &knight, &bishop, &queen, &king, &pawn
    ];

    for (i, piece) in dynamic_pieces.iter().enumerate() {
        let base = piece.base();
        println!("   Piece {} - {} {}", 
            i + 1, base.color, base.piece_type);
    }

    println!("\n4. Board representation:");
    
    // Create an 8x8 board (like C++ std::array<std::array<Piece, 8>, 8>)
    let mut board = create_empty_board();
    
    // Place pieces on the board
    board[0][0] = PieceVariant::Rook(Rook::new(Color::White, Position::new(0, 0)));
    board[0][1] = PieceVariant::Knight(Knight::new(Color::White, Position::new(0, 1)));
    board[0][3] = PieceVariant::Queen(Queen::new(Color::White, Position::new(0, 3)));
    board[0][4] = PieceVariant::King(King::new(Color::White, Position::new(0, 4)));
    
    // Count non-empty squares
    let mut piece_count = 0;
    for row in 0..ROWS {
        for col in 0..COLUMNS {
            if !board[row][col].is_empty() {
                piece_count += 1;
            }
        }
    }
    println!("   Board has {} pieces placed", piece_count);

    println!("\n5. Move validation examples:");
    
    let test_queen = Queen::new(Color::White, Position::new(3, 3));
    
    let moves_to_test = vec![
        (Position::new(3, 7), "horizontal"),
        (Position::new(7, 3), "vertical"),
        (Position::new(6, 6), "diagonal"),
        (Position::new(5, 4), "knight-like (invalid)"),
    ];
    
    for (target, description) in moves_to_test {
        let valid = test_queen.is_valid_basic_move(target, None);
        println!("   Queen (3,3) -> ({},{}) [{}]: {}",
            target.row, target.col, description,
            if valid { "✓ Valid" } else { "✗ Invalid" }
        );
    }

    println!("\n6. Comparison with C++:");
    println!("   ✓ C++ CRTP              → Rust traits with static dispatch");
    println!("   ✓ C++ virtual inheritance → Rust trait objects (&dyn Trait)");
    println!("   ✓ C++ std::variant      → Rust enum (native!)");
    println!("   ✓ C++ std::visit        → Rust match expression");
    println!("   ✓ C++ template<IsEmpty> → Rust generic Fn(Position) -> bool");
    
    println!("\n=== All 6 piece types implemented! ===");
}
