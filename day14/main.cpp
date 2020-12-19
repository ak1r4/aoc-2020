#include "../utils.h"

#include <bitset>
#include <unordered_map>
#include <vector>

template<std::size_t N, typename Fn>
void bt(std::uint64_t masked_loc, std::bitset<N>& tmp_mask, std::size_t pos, const std::vector<std::size_t>& float_locs, const Fn& fn) {
    if (pos == float_locs.size()) {
        fn(masked_loc | tmp_mask.to_ullong());
        return;
    }
    for (bool v : { true, false }) {
        tmp_mask.set(float_locs[pos], v);
        bt<N>(masked_loc, tmp_mask, pos + 1, float_locs, fn);
        tmp_mask.set(float_locs[pos], v ? false : true);
    }
}

int main(int, char** argv) {
    auto part_1 = [](std::string_view s) {
        std::bitset<36> mask0;
        std::bitset<36> mask1;
        std::unordered_map<int, std::int64_t> mem;
        while (!s.empty()) {
            auto endl = s.find('\n');
            if (s.substr(0, 4) == "mask") {
                mask0.set();
                mask1.reset();
                auto mask_str = s.substr(s.find('=') + 2, 36);
                for (std::size_t i = 0; i < 36; ++i) {
                    const auto& c = mask_str[35 - i];
                    if (c != 'X') {
                        mask0.set(i, 0);
                        mask1.set(i, c == '1' ? true : false);
                    }
                }
            } else {
                auto loc = std::stoi(s.substr(4, s.find(']') - 4).data());
                auto loc_eq = s.find('=');
                std::uint64_t number = std::stoull(s.substr(loc_eq + 2, endl - loc_eq - 2).data());
                mem[loc] = (number & mask0.to_ullong()) | mask1.to_ullong();
            }
            s.remove_prefix(endl + 1);
        }

        std::int64_t result = 0;
        for (const auto& [loc, value] : mem) {
            result += value;
        }
        return result;
    };

    auto part_2 = [](std::string_view s) {
        std::bitset<36> mask0;
        std::bitset<36> mask;
        std::vector<std::size_t> float_locs;
        std::unordered_map<std::uint64_t, std::uint64_t> mem;

        while (!s.empty()) {
            auto endl = s.find('\n');
            if (s.substr(0, 4) == "mask") {
                mask0.set();
                mask.reset();
                float_locs.clear();
                auto mask_str = s.substr(s.find('=') + 2, 36);
                for (std::size_t i = 0; i < 36; ++i) {
                    const auto& c = mask_str[35 - i];
                    if (c == 'X') {
                        float_locs.push_back(i);
                        mask0.set(i, false);
                    } else if (c == '1') {
                        mask.set(i, true);
                    }
                }
            } else {
                std::uint64_t loc = std::stoull(s.substr(4, s.find(']') - 4).data());
                auto loc_eq = s.find('=');
                std::uint64_t number = std::stoull(s.substr(loc_eq + 2, endl - loc_eq - 2).data());
                std::bitset<36> tmp_mask;
                bt<36>((loc & mask0.to_ullong()) | mask.to_ullong(), tmp_mask, 0, float_locs, [&](std::uint64_t new_loc) {
                    mem[new_loc] = number;
                });
            }
            s.remove_prefix(endl + 1);
        }
        std::uint64_t result = 0;
        for (const auto& [loc, value] : mem) {
            result += value;
        }
        return result;
    };
    run(part_1, argv[1], "Part 1");
    run(part_2, argv[1], "Part 2");

    return 0;
}
