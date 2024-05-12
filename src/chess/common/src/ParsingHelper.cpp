#include "common/include/ParsingHelper.hpp"
#include <fstream>

namespace mlp_ha {

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
        if (text[firstNonSpace] != ' ') {
            break;
        }
        ++firstNonSpace;
    }
    return firstNonSpace;
}

size_t GetNextSpace(std::string_view text, size_t index) {
    int firstNonSpace = index;
    while (firstNonSpace < text.size()) {
        if (text[firstNonSpace] == ' ') {
            break;
        }
        ++firstNonSpace;
    }
    return firstNonSpace;
}

void GetComment(std::string_view text, std::string &comment, size_t &index) {
    // std::cout << "GetComment:=" << text << std::endl;
    int firstNonSpace = GetNextNonSpace(text, index);
    // std::cout << "GetComment:=" << firstNonSpace << std::endl;
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

// TODO comment after ;
// TODO remain without " " and next line doesn't start with " "
ParsingHelper ParseFile(const std::filesystem::path &path) {
    std::ifstream file(path);

    std::queue<std::string> q;
    if (!file.is_open()) {
        // std::cerr << "Error opening the file" << std::endl;
        return ParsingHelper{};
    }

    std::string aline;
    size_t round = 2;
    size_t begin = 0;
    size_t end = 0;
    std::string remain = "";
    while (std::getline(file, aline)) {
        if (!aline.empty()) {
            if (aline[0] == '[') {
                // std::cout << "tag:=" << aline << std::endl;
                continue;
            }
            auto line = remain + " " + aline;
            std::string to_find = std::to_string(round) + ".";
            size_t found = line.find(to_find);
            while (found != std::string::npos) {
                auto sub = line.substr(begin, found - begin);
                if (IsBalanced(sub)) {
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
    // std::cout << "remain:=" << remain << std::endl;
    // q.push(remain);
    return ParsingHelper{q, remain};
}
} // namespace helper
} // namespace mlp_ha
