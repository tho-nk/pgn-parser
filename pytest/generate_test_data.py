import chess.pgn
import os

def format_board(board):
    # Mapping of pieces to their color-coded abbreviations
    piece_symbols = {
        chess.PAWN: 'P',
        chess.KNIGHT: 'N',
        chess.BISHOP: 'B',
        chess.ROOK: 'R',
        chess.QUEEN: 'Q',
        chess.KING: 'K'
    }
    rows = []
    for rank in range(8, 0, -1):  # 8 to 1 for ranks from top to bottom
        row = []
        for file in range(1, 9):  # 1 to 8 for files from left to right
            square = chess.square(file - 1, rank - 1)
            piece = board.piece_at(square)
            
            if piece:
                color_prefix = 'w' if piece.color == chess.WHITE else 'b'
                row.append(color_prefix + piece_symbols[piece.piece_type])
            else:
                row.append("  ")  # Empty square

        rows.append("|".join(row))
    
    # Print the board in the specified format
    # for row in rows:
    #     print(row)
    # print()
    # Join all rows into a single formatted string with line breaks
    return "\n".join(rows)

# Specify the input PGN file and output directory
input_pgn_file = "Abdusattorov.pgn"  # Your multi-game PGN file
output_dir = "Abdusattorov"  # Directory to store individual game files

# Create the output directory if it doesn't exist
os.makedirs(output_dir, exist_ok=True)

# Open the multi-game PGN file
with open(input_pgn_file) as pgn_file:
    game_count = 0
    while True:
        # Read each game from the PGN file
        game = chess.pgn.read_game(pgn_file)
        if game is None:
            break  # Exit loop if no more games are found

        # Define the output filename for each game
        game_file_path = os.path.join(output_dir, f"game_{game_count + 1}.pgn")

        # Write each game to a separate PGN file
        with open(game_file_path, "w") as game_file:
            game_file.write(str(game))

        # Process to get the final board state of each game
        board = game.end().board()  # Get the board state at the end of the game
        formatted_board = format_board(board)  # Get the formatted board
        # Print or save the final board state for each game
        # print(f"Final state for game {game_count + 1}:")
        # print(board)

        # Save the formatted final board state in a separate file
        final_state_path = os.path.join(output_dir, f"game_{game_count + 1}.txt")
        with open(final_state_path, "w") as final_state_file:
            final_state_file.write(formatted_board)

        game_count += 1

print(f"Processed {game_count} games and saved them in '{output_dir}'")
