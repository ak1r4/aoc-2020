#include "../utils.h"
#include <bits/stdc++.h>
using namespace std;

struct Rule {
    Rule(string_view s) {
        auto name_delim = s.find(':');
        this->name_ = s.substr(0, name_delim);
        s.remove_prefix(name_delim + 2);
        while (!s.empty()) {
            auto rule_delim = s.find("or");
            auto rule_s = s.substr(0, rule_delim - 1);
            if (rule_delim == s.npos) {
                rule_s = s.substr(0);
            }
            auto delim = rule_s.find('-');
            auto lower_bound = stoi(rule_s.substr(0, delim).data());
            auto upper_bound = stoi(rule_s.substr(delim + 1).data());
            this->valid_ranges_.emplace_back(lower_bound, upper_bound);
            s.remove_prefix(rule_delim != s.npos ? rule_delim + 3 : rule_s.size());
        }
    }

    bool is_valid(int v) const {
        int invalid_count = 0;
        for (const auto& [lower, upper] : this->valid_ranges_) {
            if (v < lower || v > upper) {
                ++invalid_count;
            }
        }

        if (invalid_count == this->valid_ranges_.size()) {
            return false;
        }
        return true;
    }

    inline string_view name() const {
        return this->name_;
    }

private:
    string_view name_;
    vector<pair<int, int>> valid_ranges_;

};

vector<Rule> get_rules(string_view& s) {
    vector<Rule> rules;
    while(true) {
        auto endl = s.find('\n');
        if (endl != 0) {
            rules.push_back(Rule(s.substr(0, endl)));
        }
        s.remove_prefix(endl + 1);
        if (endl == 0) {
            break;
        }
    }
    return rules;
}

vector<int> get_ticket(string_view s) {
    vector<int> ticket;
    while (!s.empty()) {
        auto delim = s.find(',');
        ticket.push_back(stoi(delim != s.npos ? s.substr(0, delim).data() : s.substr(0).data()));
        s.remove_prefix(delim == s.npos ? s.size() : delim + 1);
    }
    return ticket;
}

bool ticket_is_valid(const vector<int>& ticket, const vector<Rule>& rules) {
    for (const auto& v : ticket) {
        int invalid_count = 0;
        for (const auto& rule : rules) {
            if (!rule.is_valid(v)) {
                ++invalid_count;
            }
        }
        if (invalid_count == rules.size()) {
            return false;
        }
    }
    return true;
}

int main(int, char** argv) {
    auto part_1 = [](string_view s) {
        // parse rules
        auto rules = get_rules(s);

        // skip "your ticket" second and "nearby ticket" line
        skip_lines(s, 4);

        // check nearby tickets
        int error_rate = 0;
        while (!s.empty()) {
            auto endl = s.find('\n');
            auto ticket = get_ticket(s.substr(0, endl));
            for (const auto& v : ticket) {
                int invalid_count = 0;
                for (const auto& r : rules) {
                    if (!r.is_valid(v)) {
                        ++invalid_count;
                    }
                }

                if (invalid_count == rules.size()) {
                    error_rate += v;
                }
            }
            s.remove_prefix(endl + 1);
        }

        return error_rate;
    };
    auto part_2 = [](std::string_view s) {
        auto rules = get_rules(s);

        skip_lines(s, 1);

        // get your ticket
        auto endl = s.find('\n');
        auto your_ticket = get_ticket(s.substr(0, endl));
        s.remove_prefix(endl + 1);

        skip_lines(s, 2);

        vector<vector<int>> ticket_fields_data(rules.size());
        while (!s.empty()) {
            endl = s.find('\n');
            auto ticket = get_ticket(s.substr(0, endl));
            if (ticket_is_valid(ticket, rules)) {
                for (int i = 0; i < ticket.size(); ++i) {
                    ticket_fields_data[i].push_back(ticket[i]);
                }
            }
            s.remove_prefix(endl + 1);
        }

        vector<unordered_set<int>> rules_valid_fields(rules.size());
        for (int i = 0; i < rules.size(); ++i) {
            for (int j = 0; j < ticket_fields_data.size(); ++j) {
                bool valid = true;
                for (const auto& v : ticket_fields_data[j]) {
                    if (!rules[i].is_valid(v)) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    rules_valid_fields[i].insert(j);
                }
            }
        }

        while (true) {
            bool all_done = true;
            for (const auto& fields : rules_valid_fields) {
                if (fields.size() > 1) {
                    all_done = false;
                    break;
                }
            }
            if (all_done) {
                break;
            }

            for (int i = 0; i < rules_valid_fields.size(); ++i) {
                if (rules_valid_fields[i].size() == 1) {
                    const auto key = *rules_valid_fields[i].cbegin();
                    for (int j = 0; j < rules_valid_fields.size(); ++j) {
                        if (i == j) {
                            continue;
                        }
                        if (rules_valid_fields[j].find(key) != rules_valid_fields[j].end()) {
                            rules_valid_fields[j].erase(key);
                        }
                    }
                }
            }
        }

        int64_t ans = 1;
        for (int i = 0; i < rules.size(); ++i) {
            if (rules[i].name().size() >= 9 && rules[i].name().substr(0, 9) == "departure") {
                auto pos = *rules_valid_fields[i].cbegin();
                ans *= static_cast<int64_t>(your_ticket[*rules_valid_fields[i].cbegin()]);
            }
        }

        return ans;
    };

    run(part_1, argv[1], "Part 1");
    run(part_2, argv[1], "Part 2");
    return 0;
}
