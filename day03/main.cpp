#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cmath>
#include <cstdint>

std::vector<std::string> get_full_map(const std::vector<std::string>& base_map, int right, int down) {
    std::vector<std::string> full_map;
    int h = base_map.size();
    int w = base_map[0].size();
    int n_copies = std::ceil(static_cast<double>(h) * right / (w * down));

    for (const auto& l : base_map) {
        std::string full_l = "";
        for (int i = 0; i < n_copies; ++i) {
            full_l += l;
        }
        full_map.push_back(full_l);
    }

    return full_map;
}

std::int64_t traverse_map(const std::vector<std::string>& base_map, std::vector<std::pair<int, int>>& slopes) {
    std::int64_t result = 1;

    for (const auto& slope : slopes) {
        auto full_map = get_full_map(base_map, slope.first, slope.second);

        int x = 0, y = 0;
        int n_trees = 0;

        while (x < full_map.size()) {
            if (full_map[x][y] == '#') {
                ++n_trees;
            }
            x += slope.second;
            y += slope.first;
        }
        result *= static_cast<std::int64_t>(n_trees);
    }

    return result;
}

std::vector<std::pair<int, int>> part1 = {
    {3, 1},
};

std::vector<std::pair<int, int>> part2 = {
    {1, 1},
    {3, 1},
    {5, 1},
    {7, 1},
    {1, 2},
};

int main() {
    std::fstream input("input.txt");
    std::vector<std::string> base_map;
    std::vector<std::string> full_map;

    std::string line;

    while (std::getline(input, line)) {
        base_map.push_back(line);
    }

    std::cout << "Part 1: " << traverse_map(base_map, part1) << "\n";
    std::cout << "Part 2: " << traverse_map(base_map, part2) << "\n";

    return 0;
};
