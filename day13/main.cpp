#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <chrono>
#include <limits>
#include <cmath>
#include <vector>
#include <utility>

using namespace std::chrono;

int main(int, char** argv) {
    std::fstream input(argv[1]);
    std::stringstream buf;
    buf << input.rdbuf();
    std::string str = buf.str();

    auto t0 = high_resolution_clock::now();
    std::string_view s = str;

    auto endl = s.find('\n');
    auto earliest_tm = std::stol(s.substr(0, endl).data());
    s.remove_prefix(endl + 1);

    int min_wait = std::numeric_limits<int>::max();
    int min_depart = 0;

    while (!s.empty()) {
        auto delimiter = s.find(',');
        if (delimiter == s.npos) {
            delimiter = s.find('\n');
        }
        auto depart_str = s.substr(0, delimiter);

        if (depart_str[0] != 'x') {
            auto depart = std::stol(depart_str.data());
            int wait = (std::ceil(earliest_tm / static_cast<double>(depart)) * depart) - earliest_tm;
            if (wait < min_wait) {
                min_wait = wait;
                min_depart = depart;
            }
        }

        s.remove_prefix(delimiter + 1);
    }
    auto part_1 = min_wait * min_depart;
    auto t1 = high_resolution_clock::now();

    std::cout << "Part 1: " << part_1
              << "(Lapse " << duration_cast<nanoseconds>(t1 - t0).count() << " ns)\n";

    auto t2 = high_resolution_clock::now();
    s = str;
    s.remove_prefix(s.find('\n') + 1);
    std::int64_t i = 0;
    std::vector<std::pair<std::int64_t, std::int64_t>> departs;
    while (!s.empty()) {
        auto delim = s.find(',');
        if (delim == s.npos) {
            delim = s.find('\n');
        }
        auto depart_str = s.substr(0, delim);
        if (depart_str[0] != 'x') {
            departs.emplace_back(std::stoi(depart_str.data()), i);
        }
        s.remove_prefix(delim + 1);
        ++i;
    }

    std::int64_t tm = 0;
    std::int64_t step = 1;
    for (const auto& [depart, offset] : departs) {
        while ((tm + offset) % depart != 0) {
            tm += step;
        }
        step *= depart;
    }

    auto t3 = high_resolution_clock::now();
    std::cout << "Part 2: " << tm
              << "(Lapse " << duration_cast<nanoseconds>(t3 - t2).count() << " ns)\n";

    return 0;
 }
