use pgn_parser::{parse_file, parse_text};
use std::path::Path;

fn main() {
    println!("=== PGN Parser Demo ===\n");
    
    // Demo 1: Parse sample text
    demo_parse_text();
    
    // Demo 2: Parse file (if available)
    demo_parse_file();
}

fn demo_parse_text() {
    println!("Demo 1: Parsing PGN text\n");
    
    let sample_pgn = r#"
[Event "Casual Game"]
[Site "Online"]
[Date "2024.01.15"]
[Round "1"]
[White "Player A"]
[Black "Player B"]
[Result "1-0"]

1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 Nf6 5. O-O Be7 6. Re1 b5 7. Bb3 d6 8. c3 O-O 9. h3 Nb8 10. d4 Nbd7
11. c4 c6 12. cxb5 axb5 13. Nc3 Bb7 14. Bg5 b4 15. Nb1 h6 16. Bh4 c5 17. dxe5
Nxe4 18. Bxe7 Qxe7 19. exd6 Qf6 20. Nbd2 Nxd6 21. Nc4 Nxc4 22. Bxc4 Nb6
23. Ne5 Rae8 24. Bxf7+ Rxf7 25. Nxf7 Rxe1+ 26. Qxe1 Kxf7 27. Qe3 Qg5 28. Qxg5
hxg5 29. b3 Ke6 30. a3 Kd6 31. axb4 cxb4 32. Ra5 Nd5 33. f3 Bc8 34. Kf2 Bf5
35. Ra7 g6 36. Ra6+ Kc5 37. Ke1 Nf4 38. g3 Nxh3 39. Kd2 Kb5 40. Rd6 Kc5 41. Ra6
Nf2 42. g4 Bd3 43. Re6 1-0
"#;
    
    let result = parse_text(sample_pgn);
    
    println!("Parsed {} rounds:", result.round_queue.len());
    for (i, round) in result.round_queue.iter().enumerate() {
        println!("  Round {}: {}", i + 1, round);
    }
    
    println!("\nLast run (game result):");
    println!("  {}", result.last_run.trim());
    
    println!("\n{}", "=".repeat(60));
    println!();
}

fn demo_parse_file() {
    println!("Demo 2: Parsing PGN file\n");
    
    // Check if test file exists
    let test_paths = [
        "pytest/Abdusattorov.pgn",
        "pytest/Abdusattorov/game_1.pgn",
        "../pytest/Abdusattorov.pgn",
    ];
    
    for path_str in &test_paths {
        let path = Path::new(path_str);
        if path.exists() {
            println!("Found PGN file: {}", path_str);
            
            match parse_file(path) {
                Ok(result) => {
                    println!("Successfully parsed!");
                    println!("  Total rounds: {}", result.round_queue.len());
                    
                    if !result.round_queue.is_empty() {
                        println!("\n  First 3 rounds:");
                        for (i, round) in result.round_queue.iter().take(3).enumerate() {
                            println!("    {}: {}", i + 1, round);
                        }
                    }
                    
                    println!("\n  Game result: {}", result.last_run.trim());
                }
                Err(e) => {
                    eprintln!("Error parsing file: {}", e);
                }
            }
            
            println!("\n{}", "=".repeat(60));
            return;
        }
    }
    
    println!("No test PGN files found. Tried:");
    for path in &test_paths {
        println!("  - {}", path);
    }
    println!("\nYou can test with your own PGN files by passing the path as argument.");
    println!("{}", "=".repeat(60));
}
