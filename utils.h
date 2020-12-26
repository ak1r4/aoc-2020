#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <chrono>
#include <iostream>

using namespace std::chrono;

std::string_view strip_str(std::string_view str) {
    auto pos_start = str.find_first_not_of(' ');

    return str.substr(
        pos_start,
        str.find_last_not_of(' ') - pos_start + 1
    );
}

std::vector<std::string_view> split_str(std::string_view str, char delimiter) {
    std::vector<std::string_view> splitted;
    while (true) {
        if (auto pos = str.find(delimiter); pos != std::string_view::npos) {
            splitted.push_back(strip_str(str.substr(0, pos)));
            str.remove_prefix(pos + 1);
        } else {
            if (!str.empty()) {
                splitted.push_back(strip_str(str));
            }
            break;
        }
    }

    return splitted;
}

std::pair<std::string_view, std::string_view> split_str_2(std::string_view str, char delimiter) {
    if (auto pos = str.find(delimiter); pos != std::string_view::npos) {
        return std::make_pair(
            strip_str(str.substr(0, pos)),
            strip_str(str.substr(pos + 1, str.size() - pos + 2))
        );
    }
    return std::make_pair(str, "");
}

bool is_number(std::string_view str) {
    for (const auto& c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

void skip_lines(std::string_view& s, std::size_t n) {
    for (size_t i = 0; i < n; ++i) {
        s.remove_prefix(s.find('\n') + 1);
    }
}

std::string read_file(const char* filename) {
    std::fstream fn(filename);
    std::stringstream buf;
    buf << fn.rdbuf();
    return buf.str();
}

template<typename Fn>
void run(const Fn& fn, const char* input_file, const char* msg) {
    auto str = read_file(input_file);
    std::string_view s = str;
    auto t0 = high_resolution_clock::now();
    auto result = fn(s);
    auto t1 = high_resolution_clock::now();
    std::cout << msg << ": " << result << " (Lapse: " << duration_cast<nanoseconds>(t1 - t0).count() << " ns)\n";
}
