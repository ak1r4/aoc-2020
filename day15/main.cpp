#include "../utils.h"

#include <deque>
#include <unordered_map>

//
// 0, 3, 6, 0, 3, 3, 1, 0,
int main(int, char** argv) {
    int target_idx = 2020;

    auto part_1 = [&](std::string_view s) {
        std::unordered_map<int, std::deque<std::size_t>> prev_num_idxes;

        std::size_t i = 0;
        int prev_num;
        while (!s.empty()) {
            auto delim = s.find(',');
            if (delim == s.npos) {
                delim = s.find('\n');
            }
            auto n = std::stoi(s.substr(0, delim).data());
            prev_num_idxes[n].push_back(i);
            ++i;
            prev_num = n;
            s.remove_prefix(delim + 1);
        }

        while (i < target_idx) {
            int num = 0;
            if (auto it = prev_num_idxes.find(prev_num); it != prev_num_idxes.end()) {
                num = it->second.back() - it->second.front();
            }
            if (prev_num_idxes[num].size() == 2) {
                prev_num_idxes[num].pop_front();
            }
            prev_num_idxes[num].push_back(i);
            ++i;
            prev_num = num;
        }
        return prev_num;
    };
    run(part_1, argv[1], "Part 1");
    target_idx = 30000000;
    run(part_1, argv[1], "Part 2");

}
