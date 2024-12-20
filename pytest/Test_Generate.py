import subprocess
import pytest
import os

# Replace 'your_binary' with the path to your binary
binary_path = "./pgn_parser"

# Specify the directory containing the PGN files
input_dir = "Abdusattorov"

# List to store the names of all PGN files without the .pgn extension
pgn_file_names = []

# Iterate over the files in the specified directory
for filename in os.listdir(input_dir):
    # Check if the file ends with .pgn
    if filename.endswith(".pgn"):
        # Remove the .pgn extension and add to the list
        pgn_file_names.append(input_dir+"/"+os.path.splitext(filename)[0])

# Print the list of PGN file names without the extension
# print(pgn_file_names)

def run_chess(arg):
    print("test:" + arg)
    process = subprocess.Popen([binary_path, arg], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return stdout.decode('utf-8')


@pytest.mark.parametrize("test_case", pgn_file_names)
def test_parsing_pgn(test_case):
    print("test_case:=" + test_case)
    stdout_txt = run_chess(test_case + ".pgn")
    with open(test_case + ".txt", 'r') as file:
        file_txt = file.read()
    print("stdout:=")
    print(stdout_txt)
    # print("filetxt:=")
    # print(file_txt)
    assert(file_txt.rstrip("\n") == stdout_txt.rstrip("\n"))
