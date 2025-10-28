use std::collections::VecDeque;

/// Check if parentheses and braces are balanced
/// Maps to C++ helper::IsBalanced(std::string_view)
/// 
/// PGN files use:
/// - () for variations
/// - {} for comments
pub fn is_balanced(text: &str) -> bool {
    let mut paren_count = 0i32;
    let mut curly_count = 0i32;
    
    for ch in text.chars() {
        match ch {
            '(' => paren_count += 1,
            ')' => paren_count -= 1,
            '{' => curly_count += 1,
            '}' => curly_count -= 1,
            _ => {}
        }
        
        // Early exit if we go negative (unbalanced)
        if paren_count < 0 || curly_count < 0 {
            return false;
        }
    }
    
    paren_count == 0 && curly_count == 0
}

/// Find the next non-whitespace character position
/// Maps to C++ helper::GetNextNonSpace(std::string_view, size_t)
pub fn get_next_non_space(text: &str, index: usize) -> usize {
    text[index..]
        .char_indices()
        .find(|(_, ch)| !ch.is_whitespace())
        .map(|(i, _)| index + i)
        .unwrap_or(text.len())
}

/// Find the next whitespace character position
/// Maps to C++ helper::GetNextSpace(std::string_view, size_t)
pub fn get_next_space(text: &str, index: usize) -> usize {
    text[index..]
        .char_indices()
        .find(|(_, ch)| ch.is_whitespace())
        .map(|(i, _)| index + i)
        .unwrap_or(text.len())
}

/// Extract comments (including variations and annotations) from text
/// Maps to C++ helper::GetComment(std::string_view, std::string&, size_t&)
/// 
/// PGN comments can be:
/// - (variation text)
/// - {text comment}
/// - $123 (numeric annotation glyph - NAG)
pub fn get_comment(text: &str, index: &mut usize) -> String {
    let mut idx = get_next_non_space(text, *index);
    let start_index = idx;
    
    while idx < text.len() {
        let ch = text.chars().nth(idx).unwrap_or('\0');
        
        if ch == '(' || ch == '{' {
            // Find matching closing bracket
            let closing = if ch == '(' { ')' } else { '}' };
            
            if let Some(found) = text[idx + 1..].find(closing) {
                idx = get_next_non_space(text, idx + 1 + found + 1);
            } else {
                idx += 1;
            }
        } else if ch == '$' {
            // Numeric annotation glyph (NAG) - e.g., $1, $2, etc.
            idx += 1;
            while idx < text.len() {
                let digit_or_space = text.chars().nth(idx).unwrap_or('\0');
                if digit_or_space.is_ascii_digit() || digit_or_space.is_whitespace() {
                    idx += 1;
                } else {
                    break;
                }
            }
        } else {
            break;
        }
    }
    
    let comment = text[start_index..idx].to_string();
    *index = get_next_non_space(text, idx);
    comment
}

/// Trim whitespace from the left (start) of a string
/// Maps to C++ helper::LeftTrim(std::string&)
pub fn left_trim(s: &mut String) {
    *s = s.trim_start().to_string();
}

/// Trim whitespace from the right (end) of a string
/// Maps to C++ helper::RightTrim(std::string&)
pub fn right_trim(s: &mut String) {
    *s = s.trim_end().to_string();
}

/// Trim whitespace from both ends of a string
/// Maps to C++ helper::TrimSpace(std::string&)
pub fn trim_space(s: &mut String) {
    *s = s.trim().to_string();
}

/// Remove unwanted characters from move notation
/// Maps to C++ helper::removeUnwantedChars(std::string&)
/// 
/// Removes:
/// - '+' (check)
/// - '#' (checkmate)
/// - '!' and '?' (move annotations)
/// - '\n' (newlines)
pub fn remove_unwanted_chars(s: &mut String) {
    s.retain(|c| c != '+' && c != '#' && c != '!' && c != '?' && c != '\n');
}

/// Remove "..." prefix (used for black's first move when starting mid-game)
/// Maps to C++ helper::Remove3Dot(std::string&)
/// 
/// Example: "1...e5" becomes "e5"
pub fn remove_3_dot(s: &mut String) {
    if let Some(pos) = s.find("...") {
        *s = s[pos + 3..].to_string();
    }
}

/// Process game comments by finding the first complete round
/// Maps to C++ helper::ProcessGameComment(std::string&)
/// 
/// This removes any initial game metadata/comments and finds the start of moves
pub fn process_game_comment(s: &mut String) {
    let mut first_dot = s.find('.');
    
    while let Some(dot_pos) = first_dot {
        if !is_balanced(&s[..dot_pos]) {
            // Not balanced yet, keep looking
            first_dot = s[dot_pos + 1..].find('.').map(|p| p + dot_pos + 1);
        } else {
            // Found balanced section, now move back to find the round number
            let mut move_back = dot_pos;
            
            while move_back > 0 && s.chars().nth(move_back - 1).map_or(false, |c| c.is_ascii_digit()) {
                move_back -= 1;
            }
            
            *s = s[move_back..].to_string();
            trim_space(s);
            return;
        }
    }
}

/// Result of parsing a PGN file
/// Maps to C++ struct ParsingHelper
#[derive(Debug, Clone)]
pub struct ParsingResult {
    /// Queue of parsed rounds (each round is a string like "1. e4 e5")
    pub round_queue: VecDeque<String>,
    /// Last remaining text (usually the game result)
    pub last_run: String,
}

impl ParsingResult {
    pub fn new() -> Self {
        Self {
            round_queue: VecDeque::new(),
            last_run: String::new(),
        }
    }
}

impl Default for ParsingResult {
    fn default() -> Self {
        Self::new()
    }
}

/// Parse a PGN file and extract rounds
/// Maps to C++ helper::ParseFile(const std::filesystem::path&)
/// 
/// PGN format structure:
/// ```
/// [Event "World Championship"]
/// [Site "Moscow"]
/// ...
/// 
/// 1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 ...
/// ```
pub fn parse_file(path: &std::path::Path) -> std::io::Result<ParsingResult> {
    use std::fs::File;
    use std::io::{BufRead, BufReader};
    
    let file = File::open(path)?;
    let reader = BufReader::new(file);
    
    let mut queue = VecDeque::new();
    let mut round = 2usize;
    let mut remain = String::new();
    
    for line_result in reader.lines() {
        let aline = line_result?;
        
        if aline.is_empty() {
            continue;
        }
        
        // Skip metadata tags (lines starting with '[')
        if aline.starts_with('[') {
            continue;
        }
        
        // Concatenate with previous remaining text
        let mut line = if remain.is_empty() {
            aline
        } else {
            format!("{} {}", remain, aline)
        };
        
        // Look for round markers like "2.", "3.", etc.
        let mut to_find = format!("{}.", round);
        let mut found = line.find(&to_find);
        
        while let Some(pos) = found {
            let sub = &line[..pos];
            
            if is_balanced(sub) {
                let mut sub_str = sub.trim().to_string();
                
                // Process first round specially (remove game comments)
                if round == 2 {
                    process_game_comment(&mut sub_str);
                }
                
                queue.push_back(sub_str);
                line = line[pos..].to_string();
                round += 1;
                to_find = format!("{}.", round);
                found = line.find(&to_find);
            } else {
                // Not balanced yet, keep looking
                found = line[pos + 1..].find(&to_find).map(|p| p + pos + 1);
            }
        }
        
        remain = line;
    }
    
    Ok(ParsingResult {
        round_queue: queue,
        last_run: remain,
    })
}

/// Parse PGN text directly (without reading from file)
/// Useful for testing and processing PGN strings
pub fn parse_text(text: &str) -> ParsingResult {
    let mut queue = VecDeque::new();
    let mut round = 2usize;
    let mut remain = String::new();
    
    for aline in text.lines() {
        if aline.is_empty() {
            continue;
        }
        
        // Skip metadata tags
        if aline.starts_with('[') {
            continue;
        }
        
        let mut line = if remain.is_empty() {
            aline.to_string()
        } else {
            format!("{} {}", remain, aline)
        };
        
        let mut to_find = format!("{}.", round);
        let mut found = line.find(&to_find);
        
        while let Some(pos) = found {
            let sub = &line[..pos];
            
            if is_balanced(sub) {
                let mut sub_str = sub.trim().to_string();
                
                if round == 2 {
                    process_game_comment(&mut sub_str);
                }
                
                queue.push_back(sub_str);
                line = line[pos..].to_string();
                round += 1;
                to_find = format!("{}.", round);
                found = line.find(&to_find);
            } else {
                found = line[pos + 1..].find(&to_find).map(|p| p + pos + 1);
            }
        }
        
        remain = line;
    }
    
    ParsingResult {
        round_queue: queue,
        last_run: remain,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_is_balanced() {
        assert!(is_balanced("hello"));
        assert!(is_balanced("(variation)"));
        assert!(is_balanced("{comment}"));
        assert!(is_balanced("(var1) {comment} (var2)"));
        assert!(is_balanced(""));
        
        assert!(!is_balanced("(unbalanced"));
        assert!(!is_balanced("too many)"));
        assert!(!is_balanced("{incomplete"));
        assert!(!is_balanced("((nested)"));
    }

    #[test]
    fn test_get_next_non_space() {
        assert_eq!(get_next_non_space("   hello", 0), 3);
        assert_eq!(get_next_non_space("hello", 0), 0);
        assert_eq!(get_next_non_space("hi   there", 2), 5);
    }

    #[test]
    fn test_get_next_space() {
        assert_eq!(get_next_space("hello world", 0), 5);
        assert_eq!(get_next_space("hello", 0), 5);
    }

    #[test]
    fn test_trim_space() {
        let mut s = "  hello  ".to_string();
        trim_space(&mut s);
        assert_eq!(s, "hello");
    }

    #[test]
    fn test_remove_unwanted_chars() {
        let mut s = "Nf3+".to_string();
        remove_unwanted_chars(&mut s);
        assert_eq!(s, "Nf3");
        
        let mut s2 = "Qxe4#!!".to_string();
        remove_unwanted_chars(&mut s2);
        assert_eq!(s2, "Qxe4");
    }

    #[test]
    fn test_remove_3_dot() {
        let mut s = "1...e5".to_string();
        remove_3_dot(&mut s);
        assert_eq!(s, "e5");
        
        let mut s2 = "no dots".to_string();
        remove_3_dot(&mut s2);
        assert_eq!(s2, "no dots");
    }

    #[test]
    fn test_parse_text_simple() {
        let pgn = r#"
[Event "Test Game"]

1. e4 e5 2. Nf3 Nc6 3. Bb5
"#;
        
        let result = parse_text(pgn);
        assert_eq!(result.round_queue.len(), 2);
        assert_eq!(result.round_queue[0], "1. e4 e5");
        assert_eq!(result.round_queue[1], "2. Nf3 Nc6");
    }

    #[test]
    fn test_parse_text_with_comments() {
        let pgn = r#"
1. e4 e5 {A classic opening} 2. Nf3 Nc6 (2...Nf6 is also good) 3. Bb5
"#;
        
        let result = parse_text(pgn);
        assert_eq!(result.round_queue.len(), 2);
    }

    #[test]
    fn test_get_comment() {
        let text = "(variation) {comment} $1 e4";
        let mut index = 0;
        let comment = get_comment(text, &mut index);
        
        // Should extract all comment-like elements
        assert!(comment.contains("(variation)"));
    }
}
