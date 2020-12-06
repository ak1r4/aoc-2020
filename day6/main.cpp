#include <fstream>
#include <string>
#include <bitset>
#include <iostream>
#include <chrono>

int main() {
    std::fstream input("input.txt");
    std::string line;

    std::bitset<26> ans_1, ans_2, tmp_ans;

    auto t0 = std::chrono::high_resolution_clock::now();
    int sum_count_1 = 0, sum_count_2 = 0;

    ans_2.set();
    while (std::getline(input, line)) {
        if (line.empty()) {
            sum_count_1 += ans_1.count();
            sum_count_2 += ans_2.count();
            ans_1.reset();
            ans_2.set();
            continue;
        }
        for (const auto& c : line) {
            auto idx = static_cast<std::size_t>(c - 'a');
            ans_1.set(idx, ans_1[idx] | true);
            tmp_ans.set(idx, true);
        }
        ans_2 &= tmp_ans;
        tmp_ans.reset();
    }
    sum_count_1 += ans_1.count();
    sum_count_2 += ans_2.count();
    auto t1 = std::chrono::high_resolution_clock::now();

    std::cout << "Part 1: " << sum_count_1 << "\n";
    std::cout << "Part 2: " << sum_count_2 << "\n";
    std::cout << "Lapse: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() << "ns\n";

    return 0;
 }
