#pragma once

#include <algorithm>
#include <cstddef>
#include <deque>
#include <string>
#include <string_view>

// Retain recent console output within both an entry and byte budget.
class BoundedLog {
public:
    static constexpr std::size_t MaxEntries = 1024;
    static constexpr std::size_t MaxBytes = 1024 * 1024;
    static constexpr std::size_t MaxLineBytes = 16 * 1024;

    void push_back(std::string_view text) {
        const auto length = std::min(text.size(), MaxLineBytes);
        while (!lines.empty() &&
               (lines.size() >= MaxEntries || bytes + length > MaxBytes)) {
            bytes -= lines.front().size();
            lines.pop_front();
        }
        lines.emplace_back(text.substr(0, length));
        bytes += length;
    }

    void clear() { lines.clear(); bytes = 0; }
    auto begin() const { return lines.begin(); }
    auto end() const { return lines.end(); }
    std::size_t size() const { return lines.size(); }
    std::size_t byte_size() const { return bytes; }

private:
    std::deque<std::string> lines;
    std::size_t bytes = 0;
};
