#include <fstream>
#include <unordered_set>
#include <vector>
#include <iostream>

int main() {
    std::ifstream input("input.txt");
    std::unordered_set<int> nums_set;
    std::vector<int> nums_vec;
    int n;

    while (input >> n) {
        if (nums_set.count(2020 - n) != 0) {
            std::cout << "Part 1: " << (2020 - n) * n << "\n";
        }
        nums_set.insert(n);
        nums_vec.push_back(n);
    }

    for (int i = 0; i < nums_vec.size(); ++i) {
        for (int j = i + 1; j < nums_vec.size(); ++j) {
            if (nums_set.count(2020 - nums_vec[i] - nums_vec[j]) != 0) {
                std::cout << "Part 2: " << (2020 - nums_vec[i] - nums_vec[j]) * nums_vec[i] * nums_vec[j] << "\n";
                return 0;
            }
        }
    }

    return 0;
}
