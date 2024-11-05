import subprocess
import pytest

# Replace 'your_binary' with the path to your binary
binary_path = "./pgn_parser"
test_cases = ["./checking/diagonal-prevent-check-down",
              "./checking/diagonal-prevent-check-down2",
              "./checking/verify-horizonal-prevent-check-2",
              "./checking/verify-horizonal-prevent-check",
              "./checking/verify-Knight-prevent-check-2",
              "./checking/verify-Knight-prevent-check",
              "./checking/vertical-prevent-check-down",
              "./checking/vertical-prevent-check-up",
              "./general/Abdusattorov_Yakubboev.2015.05.14.attack_promotion",
              "./general/Abdusattorov_Nodirbek.2023.12.11.promotion_black",
              "./general/Anastasia-Nodirbek.2014.10.24_promotion",
              "./general/Arnold_Nodirbek.2014.10.26",
              "./general/EnPassant",
              "./general/Fabiano-Hikaru_Round_1_April_4_2024",
              "./general/Jumanov-Nodirbek_2014.03.02",
              "./general/Complex_PGN_with_comments"]


def run_pgn_parser(arg):
    print("test:" + arg)
    process = subprocess.Popen([binary_path, arg], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return stdout.decode('utf-8')


@pytest.mark.parametrize("test_case", test_cases)
def test_parsing_pgn(test_case):
    stdout_txt = run_pgn_parser(test_case + ".pgn")
    with open(test_case + ".txt", 'r') as file:
        file_txt = file.read()
    print("stdout:=")
    print(stdout_txt)
    # print("filetxt:=")
    # print(file_txt)
    assert(file_txt.rstrip("\n") == stdout_txt.rstrip("\n"))
