#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

void dedupe_overwrite(const std::string &path) {
    std::ifstream src(path, std::ios::binary);
    if (!src) {
        std::cerr << "Error: Cannot open file for reading\n";
        return;
    }
    
    // Read and deduplicate in one pass
    std::unordered_set<std::string> seen;
    std::vector<std::string> unique;
    std::string line;
    
    src.seekg(0, std::ios::end);
    seen.reserve(std::max<size_t>(100, src.tellg() / 40));
    src.seekg(0, std::ios::beg);
    unique.reserve(seen.bucket_count());
    
    while (std::getline(src, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (!line.empty() && seen.insert(line).second)
            unique.push_back(std::move(line));
    }
    src.close();
    
    if (unique.empty()) {
        std::cout << "Warning: No valid lines found in file\n";
        return;
    }
    
    // Write all at once
    std::ofstream dst(path, std::ios::trunc | std::ios::binary);
    if (!dst) {
        std::cerr << "Error: Cannot open file for writing\n";
        return;
    }
    
    std::string out;
    out.reserve(unique.size() * 80);
    for (const auto &l : unique)
        out += l + '\n';
    dst.write(out.data(), out.size());
    
    std::cout << "Success: File deduplicated (" << unique.size() << " unique lines)\n";
}

int main() {
    dedupe_overwrite("data.txt");
}