#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

bool read_lines(const std::string &path, std::vector<std::string> &unique_lines) {
    std::unordered_set<std::string> seen;
    std::ifstream src(path);
    
    if (!src.is_open()) {
        std::cerr << "Error: Cannot open file for reading" << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(src, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }
        if (!seen.insert(line).second) {
            continue;
        }
        unique_lines.push_back(line);
    }
    
    src.close();
    return true;
}

bool write_lines(const std::string &path, const std::vector<std::string> &lines) {
    std::ofstream dst(path, std::ios::trunc);
    
    if (!dst.is_open()) {
        std::cerr << "Error: Cannot open file for writing" << std::endl;
        return false;
    }
    
    for (const auto &line : lines) {
        dst << line << "\n";
    }
    
    dst.close();
    return true;
}

void dedupe_overwrite(const std::string &path) {
    std::vector<std::string> unique_lines;
    
    if (!read_lines(path, unique_lines)) {
        return;
    }
    
    if (!write_lines(path, unique_lines)) {
        return;
    }
    
    std::cout << "Success: File deduplicated" << std::endl;
}

int main() {
    dedupe_overwrite("data.txt");
    return 0;
}