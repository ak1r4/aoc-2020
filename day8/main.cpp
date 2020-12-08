#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <string>
#include <string_view>
#include <chrono>
#include <algorithm>
#include <stdexcept>

enum class Op : std::uint8_t {
    nop = 0,
    acc = 1,
    jmp = 2
};

using inst_vec_t = std::vector<std::pair<Op, std::int64_t>>;

std::pair<std::int64_t, int> execute(const inst_vec_t& inst_vec) {
    std::vector<bool> visited(inst_vec.size(), false);
    int inst_idx = 0;
    std::int64_t accumulator = 0;

    while(inst_idx < inst_vec.size() && !visited[inst_idx]) {
        const auto& inst = inst_vec[inst_idx];
        visited[inst_idx] = true;
        switch (inst.first) {
            case Op::nop:
                ++inst_idx;
                break;
            case Op::acc:
                accumulator += inst.second;
                ++inst_idx;
                break;
            case Op::jmp:
                inst_idx += inst.second;
                break;
        }
    }
    return std::make_pair(accumulator, inst_idx);
}

int main() {
    std::fstream input("input.txt");
    std::stringstream input_buffer;
    input_buffer << input.rdbuf();
    std::string input_str = input_buffer.str();

    auto t0 = std::chrono::high_resolution_clock::now();
    std::string_view str_view = input_str;
    inst_vec_t inst_vec;

    while (!str_view.empty()) {
        auto line = str_view.substr(0, str_view.find('\n'));
        auto split_pos = line.find(' ');
        auto opand_str = line.substr(split_pos + 1, line.size() - split_pos - 1);
        inst_vec.emplace_back(
            [](std::string_view op_str) {
                if (op_str == "nop") {
                    return Op::nop;
                } else if (op_str == "acc") {
                    return Op::acc;
                } else if (op_str == "jmp") {
                    return Op::jmp;
                } else {
                    throw std::runtime_error("Unknown op: " + std::string(op_str));
                }
            }(line.substr(0, split_pos)),
            static_cast<std::int64_t>(std::stoll(opand_str.data()))
        );
        str_view.remove_prefix(line.size() + 1);
    }

    auto result_1 = execute(inst_vec);

    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Part 1: " << result_1.first << "("
              << "lapse: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() << "ns)\n";

    // Part 2
    auto t2 = std::chrono::high_resolution_clock::now();

    std::int64_t result_2 = 0;

    for (std::size_t i = 0; i < inst_vec.size(); ++i) {
        const auto& inst = inst_vec[i];

        if (inst.first == Op::jmp) {
            inst_vec[i] = std::make_pair(Op::nop, inst.second);
            auto r = execute(inst_vec);
            if (r.second == inst_vec.size()) {
                result_2 = r.first;
                break;
            }
            inst_vec[i] = std::make_pair(Op::jmp, inst.second);
        } else if (inst.first == Op::nop) {
            inst_vec[i] = std::make_pair(Op::jmp, inst.second);
            auto r = execute(inst_vec);
            if (r.second == inst_vec.size()) {
                result_2 = r.first;
                break;
            }
            inst_vec[i] = std::make_pair(Op::nop, inst.second);
        }
    }
    auto t3 = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2: " << result_2 << "("
              << "lapse: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count() << "ns)\n";

    return 0;
}
