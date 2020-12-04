#include "../utils.h"

#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

const std::vector<std::string> REQUIRED_FIELDS = {
    "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid",
};
const std::unordered_set<std::string> VALID_EYE_COLORS = {
    "amb", "blu", "brn", "gry", "grn", "hzl", "oth",
};

int main() {
    std::fstream input("input.txt");
    std::vector<std::unordered_map<std::string, std::string>> batch;
    std::string line;
    std::unordered_map<std::string, std::string> batch_entry;

    while (std::getline(input, line)) {
        if (line.empty()) {
            batch.push_back(batch_entry);
            batch_entry.clear();
            continue;
        }
        auto fields = split_str(line, ' ');
        for (const auto& field : fields) {
            const auto [key, value] = split_str_2(field, ':');
            batch_entry[std::string(key)] = std::string(value);
        }
    }
    if (!batch_entry.empty()) {
        batch.push_back(batch_entry);
    }

    int n_valid_with_all_required_fields = 0;
    int n_valid_with_field_validation = 0;

    for (const auto& b : batch) {
        bool has_all_required_fields = true;
        for (const auto& required_field : REQUIRED_FIELDS) {
            if (b.find(required_field) == b.end()) {
                has_all_required_fields = false;
                break;
            }
        }
        if (!has_all_required_fields) {
            continue;
        }
        ++n_valid_with_all_required_fields;

        bool all_fields_are_valid = true;

        for (const auto& [k, v] : b) {
            if (k == "byr") {
                if (v.size() != 4) { all_fields_are_valid = false; break; }
                if (!is_number(v)) { all_fields_are_valid = false; break; }
                if (auto byr = std::stoll(v); (byr < 1920) || (byr > 2002)) { all_fields_are_valid = false; break; }
            } else if (k == "iyr") {
                if (v.size() != 4) { all_fields_are_valid = false; break; }
                if (!is_number(v)) { all_fields_are_valid = false; break; }
                if (auto iyr = std::stoll(v); (iyr < 2010) || (iyr > 2020)) { all_fields_are_valid = false; break; }
            } else if (k == "eyr") {
                if (v.size() != 4) { all_fields_are_valid = false; break; }
                if (!is_number(v)) { all_fields_are_valid = false; break; }
                if (auto eyr = std::stoll(v); (eyr < 2020) || (eyr > 2030)) { all_fields_are_valid = false; break; }
            } else if (k == "hgt") {
                std::string_view s = v;
                auto unit = s.substr(s.size() - 2, 2);
                if ((unit != "cm") && (unit != "in")) { all_fields_are_valid = false; break; }
                if (!is_number(s.substr(0, s.size() - 2))) { all_fields_are_valid = false; break; }
                auto number = std::stoll(std::string(s.substr(0, s.size())));
                if ((unit == "cm") && (number < 150 || number > 193)) { all_fields_are_valid = false; break; }
                if ((unit == "in") && (number < 59 || number > 76)) { all_fields_are_valid = false; break; }
            } else if (k == "hcl") {
                if (v.size() != 7) { all_fields_are_valid = false; break; }
                if (v[0] != '#') { all_fields_are_valid = false; break; }
                bool all_char_valid = true;
                for (int i = 1; i < v.size(); ++i) {
                    if (!(((v[i] >= '0') && (v[i] <= '9') || ((v[i] >= 'a') && (v[i] <= 'f'))))) {
                        all_char_valid = false;
                        break;
                    }
                }
                if (!all_char_valid) {all_fields_are_valid = false; break; }
            } else if (k == "ecl") {
                if (VALID_EYE_COLORS.count(v) == 0) { all_fields_are_valid = false; break; }
            } else if (k == "pid") {
                if (v.size() != 9) { all_fields_are_valid = false; break; }
            }
        }

        if (all_fields_are_valid) {
            ++n_valid_with_field_validation;
        }

    }

    std::cout << "Part 1: " << n_valid_with_all_required_fields << "\n";
    std::cout << "Part 2: " << n_valid_with_field_validation << "\n";
    return 0;
}
