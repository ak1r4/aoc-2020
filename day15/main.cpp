#include "../utils.h"

#include <cstring>

int reciter(std::string_view s, std::size_t target) {
    auto size_ = target * 3;
    int* num_freqs_ = new int[size_];
    int prev_num_ = -1;
    int i_ = 0;
    std::memset(num_freqs_, -1, size_ * sizeof(int));

    while (!s.empty()) {
        auto delim = s.find(',');
        if (delim == s.npos) {
            delim = s.find('\n');
        }
        auto n = std::stoul(s.substr(0, delim).data());
        num_freqs_[n * 3] = 1;
        num_freqs_[n * 3 + 1] = i_;
        ++i_;
        prev_num_ = n;
        s.remove_prefix(delim + 1);
    }

    while (i_ < target) {
        int n = 0;
        if (num_freqs_[prev_num_ * 3 + 2] != -1) {  // not first time
            n = (
                num_freqs_[prev_num_ * 3 + num_freqs_[prev_num_ * 3]] -
                num_freqs_[prev_num_ * 3 + 3 - num_freqs_[prev_num_ * 3]]
            );
        }

        if (num_freqs_[n * 3] == -1) {  // first time
            num_freqs_[n * 3] = 1;
        } else {
            num_freqs_[n * 3] = 3 - num_freqs_[n * 3];
        }
        num_freqs_[n * 3 + num_freqs_[n * 3]] = i_;
        prev_num_ = n;
        ++i_;
    }

    delete [] num_freqs_;
    return prev_num_;
}

int main(int, char** argv) {
    auto part_1 = [&](std::string_view s) {
        return reciter(s, 2020);
    };

    auto part_2 = [&](std::string_view s) {
        return reciter(s, 30000000);
    };

    run(part_1, argv[1], "Part 1");
    run(part_2, argv[1], "Part 2");

    return 0;
}
