#include "ParsingHelper.hpp"
#include <fstream>

namespace mlp_ha {

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

std::queue<std::string> ParseFile(const std::filesystem::path &path) {
    std::ifstream file(path);

    std::queue<std::string> q;
    if (!file.is_open()) {
        std::cerr << "Error opening the file" << std::endl;
        return q;
    }

    // Read and print each line of the file
    std::string aline;
    size_t round = 2;
    size_t begin = 0;
    size_t end = 0;
    std::string remain = "";
    while (std::getline(file, aline)) {
        auto line = remain + aline;
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
    q.push(remain);
    return q;
}

} // namespace mlp_ha
