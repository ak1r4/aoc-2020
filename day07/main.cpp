#include "../utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <chrono>

using bag_map_t = std::unordered_map<std::string_view, std::vector<std::pair<std::string_view, std::size_t>>>;
const char* target = "shiny gold";

bool contains_bag(std::string_view container_bag, std::string_view target_bag, const bag_map_t& bag_map) {
    for (const auto& [b, n] : bag_map.at(container_bag)) {
        if (b == target_bag || contains_bag(b, target_bag, bag_map)) {
            return true;
        }
    }
    return false;
}

int count_bags(std::string_view container_bag, const bag_map_t& bag_map) {
    if (bag_map.at(container_bag).empty()) {
        return 0;
    }

    int n_bags = 0;

    for (const auto& b : bag_map.at(container_bag)) {
        n_bags += ((count_bags(b.first, bag_map) + 1)* b.second);
    }
    return n_bags;
}

int main() {
    std::fstream input("input.txt");
    std::stringstream input_buffer;
    input_buffer << input.rdbuf();
    std::string input_str = input_buffer.str();

    // Build bag map
    auto t0 = std::chrono::high_resolution_clock::now();
    std::string_view str_view = input_str;
    bag_map_t bag_map;
    while (!str_view.empty()) {
        auto endl = str_view.find('\n');
        auto line = str_view.substr(0, endl);
        auto pos = line.find("contain");
        auto orig_key = line.substr(0, pos - 1);
        auto key = orig_key.substr(0, orig_key.rfind(' '));
        auto value_str = line.substr(pos + 8, line.size() - pos - 9);  // skip over "contain ", not include the trailing '.'
        std::vector<std::pair<std::string_view, std::size_t>> bag_contains;

        if (value_str != "no other bags") {
            auto splitted_v = split_str(value_str, ',');

            for (const auto& v : splitted_v) {
                auto [n, bag_names] = split_str_2(v, ' ');
                auto bag_name = bag_names.substr(0, bag_names.rfind(' '));
                bag_contains.push_back(
                    std::make_pair(
                        bag_name,
                        static_cast<std::size_t>(std::stoi(std::string(n)))
                    )
                );
            }
        }
        bag_map[key] = bag_contains;
        str_view.remove_prefix(endl + 1);
    }

    int n_bags = 0;
    for (const auto& [k, vs] : bag_map) {
        if (k == target) {
            continue;
        }
        if (contains_bag(k, target, bag_map)) {
            ++n_bags;
        }
    }

    int n_count = count_bags(target, bag_map);

    auto t1 = std::chrono::high_resolution_clock::now();

    std::cout << "Part 1: " << n_bags << "\n";
    std::cout << "Part 2: " << n_count << "\n";
    std::cout << "Lapse: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() << "ns\n";

    return 0;
}
