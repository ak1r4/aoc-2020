#include "../utils.h"

#include <limits>
#include <cmath>
#include <vector>
#include <utility>

int main(int, char** argv) {
    auto part_1 = [](std::string_view s) {
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
        return min_wait * min_depart;
    };

    auto part_2 = [](std::string_view s) {
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
        return tm;
    };

    run(part_1, argv[1], "Part 1");
    run(part_2, argv[1], "Part 2");

    return 0;
 }
