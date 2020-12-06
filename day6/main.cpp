#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <string_view>
#include <bitset>
#include <chrono>

int main() {
    // Read file into string
    std::ifstream input("input.txt");
    std::stringstream input_buffer;
    input_buffer << input.rdbuf();
    std::string input_str = input_buffer.str();

    // Main
    auto t0 = std::chrono::high_resolution_clock::now();

    std::string_view str_view = input_str;
    int sum_count_1 = 0, sum_count_2 = 0;
    std::bitset<26> ans_1, ans_2, tmp_ans;

    ans_2.set();
    while (!str_view.empty()) {
        auto endl = str_view.find('\n');
        auto line = str_view.substr(0, endl);

        if (line.empty()) {
            sum_count_1 += ans_1.count();
            sum_count_2 += ans_2.count();
            ans_1.reset();
            ans_2.set();
        } else {
            for (const auto& c : line) {
                auto idx = static_cast<std::size_t>(c - 'a');
                ans_1.set(idx, ans_1[idx] | true);
                tmp_ans.set(idx, true);
            }
            ans_2 &= tmp_ans;
            tmp_ans.reset();
        }

        str_view.remove_prefix(endl + 1);
    }
    sum_count_1 += ans_1.count();
    sum_count_2 += ans_2.count();

    auto t1 = std::chrono::high_resolution_clock::now();

    std::cout << "Part 1: " << sum_count_1 << "\n";
    std::cout << "Part 2: " << sum_count_2 << "\n";
    std::cout << "Lapse: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() << "ns\n";

    return 0;
 }
