#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <string_view>

// Reserve reasonable capacity for typical files
static constexpr size_t DEFAULT_RESERVE_SIZE = 10000;

bool read_lines(const std::string &path, std::vector<std::string> &unique_lines) {
    std::ifstream src(path, std::ios::binary); // Binary mode for faster reading
    
    if (!src.is_open()) {
        std::cerr << "Error: Cannot open file for reading" << std::endl;
        return false;
    }
    
    // Pre-reserve capacity to avoid rehashing and reallocation
    src.seekg(0, std::ios::end);
    size_t file_size = src.tellg();
    src.seekg(0, std::ios::beg);
    
    // Estimate line count and reserve accordingly
    size_t estimated_lines = std::max(static_cast<size_t>(100), file_size / 40);
    std::unordered_set<std::string> seen;
    seen.reserve(estimated_lines);
    unique_lines.reserve(estimated_lines);
    
    std::string line;
    line.reserve(256); // Reserve typical line size
    
    // Read in larger chunks using getline
    while (std::getline(src, line)) {
        // Remove trailing \r if present (Windows line endings)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
        if (line.empty()) {
            continue;
        }
        
        // Try to insert; if successful, add to vector
        if (seen.insert(line).second) {
            unique_lines.push_back(std::move(line));
            line.clear();
            line.reserve(256); // Re-reserve for next line
        } else {
            line.clear();
            line.reserve(256); // Re-reserve for next line
        }
    }
    
    // Shrink to fit if memory is a concern (optional)
    unique_lines.shrink_to_fit();
    
    src.close();
    return true;
}

bool write_lines(const std::string &path, const std::vector<std::string> &lines) {
    std::ofstream dst(path, std::ios::trunc | std::ios::binary);
    
    if (!dst.is_open()) {
        std::cerr << "Error: Cannot open file for writing" << std::endl;
        return false;
    }
    
    // Reserve buffer for faster writing
    constexpr size_t BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];
    dst.rdbuf()->pubsetbuf(buffer, BUFFER_SIZE);
    
    // Write all lines at once for better performance
    std::string combined;
    combined.reserve(lines.size() * 80); // Estimate average line length
    
    for (const auto &line : lines) {
        combined.append(line);
        combined.push_back('\n');
    }
    
    dst.write(combined.data(), combined.size());
    
    dst.close();
    return true;
}

// Overload for files that need to be processed in streaming mode (for very large files)
bool write_lines_streaming(const std::string &path, const std::vector<std::string> &lines) {
    std::ofstream dst(path, std::ios::trunc | std::ios::binary);
    
    if (!dst.is_open()) {
        std::cerr << "Error: Cannot open file for writing" << std::endl;
        return false;
    }
    
    constexpr size_t BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];
    dst.rdbuf()->pubsetbuf(buffer, BUFFER_SIZE);
    
    for (const auto &line : lines) {
        dst << line << '\n';
    }
    
    dst.close();
    return true;
}

void dedupe_overwrite(const std::string &path) {
    std::vector<std::string> unique_lines;
    
    if (!read_lines(path, unique_lines)) {
        return;
    }
    
    // Check if we have something to write
    if (unique_lines.empty()) {
        std::cout << "Warning: No valid lines found in file" << std::endl;
        return;
    }
    
    if (!write_lines(path, unique_lines)) {
        return;
    }
    
    std::cout << "Success: File deduplicated (" << unique_lines.size() << " unique lines)" << std::endl;
}

int main() {
    dedupe_overwrite("data.txt");
    return 0;
}