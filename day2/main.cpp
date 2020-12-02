#include "../utils.h"

#include <fstream>
#include <string>
#include <iostream>


int main() {
    std::fstream input("input.txt");

    int n_valid_1 = 0;
    int n_valid_2 = 0;
    std::string line;

    while (std::getline(input, line)) {
        auto [requirement, password] = split_str_2(line, ':');
        auto [bounds, letter] = split_str_2(requirement, ' ');
        auto [lower, upper] = split_str_2(bounds, '-');
        auto lower_i = std::stoi(lower.data());
        auto upper_i = std::stoi(upper.data());
        auto letter_c = letter[0];

        int count = 0;
        for (const auto& c : password) {
            if (c == letter_c) {
                ++count;
            }
        }

        if ((count >= lower_i) && (count <= upper_i)) {
            ++n_valid_1;
        }

        if (
            ((password[lower_i - 1] == letter_c) && (password[upper_i - 1] != letter_c)) ||
            ((password[lower_i - 1] != letter_c) && (password[upper_i - 1] == letter_c))
        ) {
            ++n_valid_2;
        }
    }

    std::cout << "Part 1: " << n_valid_1 << "\n";
    std::cout << "Part 2: " << n_valid_2 << "\n";


    return 0;

}
