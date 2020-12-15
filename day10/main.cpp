#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <chrono>
#include <unordered_map>

int main(int, char** argv) {
    std::fstream input(argv[1]);
    std::stringstream str_buf;
    str_buf << input.rdbuf();
    std::string str = str_buf.str();

    auto t0 = std::chrono::high_resolution_clock::now();
    std::string_view s = str;

    std::vector<int> adapters;
    while (!s.empty()) {
        auto endl = s.find('\n');
        adapters.push_back(std::stol(s.substr(0, endl).data()));
        s.remove_prefix(endl + 1);
    }

    std::sort(adapters.begin(), adapters.end());

    std::unordered_map<int, std::size_t> counter;

    for (int i = -1; i < static_cast<int>(adapters.size()); ++i) {
        if (i == -1) {
            counter[adapters[i + 1] - 0]++;
        } else if (i == adapters.size() - 1) {
            counter[3]++;
        } else {
            counter[adapters[i + 1] - adapters[i]]++;
        }
    }
    auto t1 = std::chrono::high_resolution_clock::now();

    std::cout << "Part 1: " << counter[3] * counter[1]
              << "(lapse: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() << " ns)\n";

    auto t2 = std::chrono::high_resolution_clock::now();

    std::unordered_map<int, std::size_t> n_comb_so_far;
    n_comb_so_far[0] = 1;
    n_comb_so_far[1] = 1;
    n_comb_so_far[2] = 1;
    n_comb_so_far[3] = 1;

    for (const auto& n : adapters) {
        for (std::size_t i = 1; i <=3; ++i) {
            n_comb_so_far[n + i] += n_comb_so_far[n];
        }
    }

    auto t3 = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2: " << n_comb_so_far[adapters.back() + 3]
              << "(lapse: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count() << " ns)\n";
    return 0;
}
