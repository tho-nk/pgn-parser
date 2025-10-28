use chess_moves::*;

fn main() {
    println!("=== Chess Move System - Rust Implementation ===\n");

    // 1. Create different move types using the factory
    println!("1. Creating moves using factory pattern:");
    
    let basic = MoveVariant::create(
        MoveType::BasicMove,
        Color::White,
        "e4".to_string(),
        "King's pawn opening".to_string(),
    );
    println!("   Created: {} - {}", basic.move_text(), basic.comment());

    let attack = MoveVariant::create(
        MoveType::AttackMove,
        Color::Black,
        "exd5".to_string(),
        "Pawn captures pawn".to_string(),
    );
    println!("   Created: {} - {}", attack.move_text(), attack.comment());

    let castling = MoveVariant::create(
        MoveType::CastlingMove,
        Color::White,
        "O-O".to_string(),
        "Kingside castling".to_string(),
    );
    println!("   Created: {} - {}", castling.move_text(), castling.comment());

    // 2. Process moves (polymorphic behavior)
    println!("\n2. Processing moves (polymorphic dispatch):");
    
    let mut moves: Vec<MoveVariant> = vec![
        MoveVariant::create(MoveType::BasicMove, Color::White, "d4".to_string(), String::new()),
        MoveVariant::create(MoveType::AttackMove, Color::Black, "Nxe4".to_string(), String::new()),
        MoveVariant::create(MoveType::PromotionMove, Color::White, "e8=Q".to_string(), String::new()),
        MoveVariant::create(MoveType::CastlingMove, Color::Black, "O-O-O".to_string(), String::new()),
        MoveVariant::create(
            MoveType::AttackPromotionMove,
            Color::White,
            "fxe8=R".to_string(),
            String::new(),
        ),
    ];

    for mv in &mut moves {
        mv.process_move();
    }

    // 3. Demonstrate trait object usage (dynamic dispatch)
    println!("\n3. Using trait objects (like C++ virtual pointers):");
    
    let basic_move = BasicMove::new(
        MoveType::BasicMove,
        Color::White,
        "Nf3".to_string(),
        String::new(),
    );
    let attack_move = AttackMove::new(
        MoveType::AttackMove,
        Color::Black,
        "Bxc3".to_string(),
        String::new(),
    );
    
    // Create a vector of trait objects (like std::vector<Move*>)
    let trait_moves: Vec<&dyn Move> = vec![&basic_move, &attack_move];
    
    for (i, mv) in trait_moves.iter().enumerate() {
        println!("   Move {}: {} (type: {:?})", i + 1, mv.move_text(), mv.move_type());
    }

    // 4. Working with Rounds
    println!("\n4. Complete game with rounds:");
    
    let mut rounds = Rounds::new();
    
    // Round 1: e4 e5
    rounds.push(Round::with_moves(
        1,
        "Italian game opening".to_string(),
        MoveVariant::create(MoveType::BasicMove, Color::White, "e4".to_string(), String::new()),
        MoveVariant::create(MoveType::BasicMove, Color::Black, "e5".to_string(), String::new()),
    ));
    
    // Round 2: Nf3 Nc6
    rounds.push(Round::with_moves(
        2,
        String::new(),
        MoveVariant::create(MoveType::BasicMove, Color::White, "Nf3".to_string(), String::new()),
        MoveVariant::create(MoveType::BasicMove, Color::Black, "Nc6".to_string(), String::new()),
    ));
    
    // Round 3: Bc4 Bc5 (Italian Opening)
    rounds.push(Round::with_moves(
        3,
        String::new(),
        MoveVariant::create(MoveType::BasicMove, Color::White, "Bc4".to_string(), String::new()),
        MoveVariant::create(MoveType::BasicMove, Color::Black, "Bc5".to_string(), String::new()),
    ));

    // Round 4: O-O Nf6
    rounds.push(Round::with_moves(
        4,
        String::new(),
        MoveVariant::create(MoveType::CastlingMove, Color::White, "O-O".to_string(), String::new()),
        MoveVariant::create(MoveType::BasicMove, Color::Black, "Nf6".to_string(), String::new()),
    ));

    // Execute all rounds
    for round in &mut rounds {
        round.run();
    }

    // 5. Demonstrate move data access
    println!("\n5. Accessing move data:");
    
    let queen_move = MoveVariant::create(
        MoveType::BasicMove,
        Color::White,
        "Qd3".to_string(),
        "Developing the queen".to_string(),
    );
    
    let data = queen_move.move_data();
    println!("   Move: {}", queen_move.move_text());
    println!("   Piece type: {:?}", data.piece_type);
    println!("   To position: ({}, {}) [row, col]", data.to_position.row, data.to_position.col);
    println!("   From position: ({}, {})", data.from_position.row, data.from_position.col);

    // 6. C++ vs Rust comparison
    println!("\n6. Architecture Comparison:");
    println!("   C++ Pattern                  →  Rust Pattern");
    println!("   ────────────────────────────────────────────────────");
    println!("   class Move {{ virtual ... }}  →  trait Move");
    println!("   class BasicMove : Move       →  struct BasicMove + impl Move");
    println!("   std::unique_ptr<Move>        →  MoveVariant enum (owned)");
    println!("   Move* (raw pointer)          →  &dyn Move (trait object)");
    println!("   std::vector<unique_ptr<>>    →  Vec<MoveVariant>");
    println!("   move_factory::CreateMove()   →  MoveVariant::create()");
    println!("   virtual void ProcessMove()   →  fn process_move(&mut self)");
    println!("   Multiple inheritance         →  Composition or enum variants");

    println!("\n=== Complete chess move system implemented! ===");
}
