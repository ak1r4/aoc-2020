#include "../utils.h"

#include <string>
#include <iostream>

int main() {
    std::string s{" 12-13 v : kvvvbmdvvvvvvcvvvv"};

    auto [requirement, password] = split_str_2(s, ':');
    auto [bounds, letter] = split_str_2(requirement, ' ');
    auto [lower, upper] = split_str_2(bounds, '-');

    std::cout << "Check letter: '" << letter << "'\n";
    std::cout << "[" << std::stoi(lower.data()) << "," << std::stoi(upper.data()) << "]\n";
    std::cout << "Password: '" << password << "'\n";

    return 0;
}
