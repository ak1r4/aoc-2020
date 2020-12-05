#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <utility>

const int MAX_ROW = 127;
const int MAX_COL = 7;

int find_pos(std::string_view str, int lo, int hi, char first_c, char second_c) {
    int mid = 0;

    for (const auto& c : str) {
        mid = lo + (hi - lo) / 2;

        if (c == first_c) {
            hi = mid;
        } else if (c == second_c) {
            lo = mid + 1;
        }
    }

    return lo;
}

int main() {
    std::fstream input("input.txt");
    std::string line;
    std::vector<int> seat_ids;

    while(std::getline(input, line)) {
        std::string_view s = line;
        auto row = find_pos(s.substr(0, 7), 0, MAX_ROW, 'F', 'B');
        auto col = find_pos(s.substr(7, 3), 0, MAX_COL, 'L', 'R');
        seat_ids.push_back(row * 8 + col);
    }

    std::sort(seat_ids.begin(), seat_ids.end());

    std::cout << "Part 1: " << seat_ids.back() << "\n";

    for (int i = 0; i < seat_ids.size() - 1; ++i) {
        auto diff = seat_ids[i + 1] - seat_ids[i];
        if (diff == 2) {
            std::cout << "Part 2: " << seat_ids[i] + 1 << "\n";
            break;
        }
    }

    return 0;
}
