#include "common/include/ParsingHelper.hpp"
#include <fstream>

namespace pgn {

namespace helper {
bool IsBalanced(std::string_view str) {
    int paren_count = 0;
    int curly_count = 0;
    for (char c : str) {
        if (c == '(')
            paren_count++;
        else if (c == ')')
            paren_count--;
        else if (c == '{')
            curly_count++;
        else if (c == '}')
            curly_count--;
        if (paren_count < 0 || curly_count < 0)
            return false;
    }
    return paren_count == 0 && curly_count == 0;
}

size_t GetNextNonSpace(std::string_view text, size_t index) {
    int firstNonSpace = index;
    while (firstNonSpace < text.size()) {
        if (!std::isspace(text[firstNonSpace])) {
            break;
        }
        ++firstNonSpace;
    }
    return firstNonSpace;
}

size_t GetNextSpace(std::string_view text, size_t index) {
    int firstNonSpace = index;
    while (firstNonSpace < text.size()) {
        if (std::isspace(text[firstNonSpace])) {
            break;
        }
        ++firstNonSpace;
    }
    return firstNonSpace;
}

void GetComment(std::string_view text, std::string &comment, size_t &index) {
    // std::clog << "[THO][I] GetComment:=" << text << std::endl;
    int firstNonSpace = GetNextNonSpace(text, index);
    // std::clog << "[THO][I] GetComment:=" << firstNonSpace << std::endl;
    int indexBegin = firstNonSpace;
    while (indexBegin < text.size()) {
        if (text[indexBegin] == '(' || text[indexBegin] == '{') {
            size_t found;
            if (text[indexBegin] == '(') {
                found = text.find(")", indexBegin + 1);
            }
            if (text[indexBegin] == '{') {
                found = text.find("}", indexBegin + 1);
            }
            indexBegin = GetNextNonSpace(text, found + 1);
        } else if (text[indexBegin] == '$') {
            do {
                ++indexBegin;
            } while (isdigit(text[indexBegin]) || std::isspace(text[indexBegin]));

        } else {
            break;
        }
    }
    comment = text.substr(index, indexBegin - index);
    index = GetNextNonSpace(text, indexBegin);
}

void Remove3Dot(std::string &str) {
    auto found = str.find("...");
    if (found != std::string::npos) {
        str = str.substr(found + 3);
    }
}

void ProcessGameComment(std::string &str) {
    auto firstDot = str.find_first_of(".");
    while (firstDot != std::string::npos) {
        if (!IsBalanced(str.substr(0, firstDot))) {
            firstDot = str.find_first_of(".", firstDot + 1);
        } else {
            int moveBack = firstDot;
            do {
                moveBack--;
            } while (moveBack >= 0 && std::isdigit(str[moveBack]));
            if (moveBack < 0) {
                moveBack++;
            }
            // std::clog << "[THO][I] game comment :" << str.substr(0, moveBack) << std::endl;
            str = str.substr(moveBack);
            TrimSpace(str);
            return;
        }
    }
}

ParsingHelper ParseFile(const std::filesystem::path &path) {
    std::ifstream file(path);
    std::queue<std::string> q;
    if (!file.is_open()) {
        std::cerr << "[THO][E] Error opening the file" << std::endl;
        return ParsingHelper{};
    }

    std::string aline;
    size_t round = 2;
    size_t begin = 0;
    std::string remain = "";
    while (std::getline(file, aline)) {
        if (!aline.empty()) {
            if (aline[0] == '[') {
                // std::clog << "[THO][I] tag:=" << aline << std::endl;
                continue;
            }
            auto line = remain + " " + aline;
            std::string to_find = std::to_string(round) + ".";
            size_t found = line.find(to_find);
            while (found != std::string::npos) {
                auto sub = line.substr(begin, found - begin);
                TrimSpace(sub);
                if (IsBalanced(sub)) {
                    if (round == 2) {
                        ProcessGameComment(sub);
                    }
                    q.push(sub);
                    line = line.substr(found);
                    ++round;
                    to_find = std::to_string(round) + ".";
                    found = line.find(to_find);
                } else {
                    found = line.find(to_find, found + 1);
                }
            }
            remain = line;
        }
    }
    // std::clog << "[THO][I] remain:=" << remain << std::endl;
    return ParsingHelper{q, remain};
}
} // namespace helper
} // namespace pgn
