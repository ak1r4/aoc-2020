#include <vector>
#include <string_view>
#include <utility>

std::string_view strip_str(std::string_view str) {
    auto pos_start = str.find_first_not_of(' ');

    return str.substr(
        pos_start,
        str.find_last_not_of(' ') - pos_start + 1
    );
}

std::vector<std::string_view> split_str(std::string_view str, char delimiter) {
    std::vector<std::string_view> splitted;
    while (true) {
        if (auto pos = str.find(delimiter); pos != std::string_view::npos) {
            splitted.push_back(strip_str(str.substr(0, pos)));
            str.remove_prefix(pos + 1);
        } else {
            if (!str.empty()) {
                splitted.push_back(strip_str(str));
            }
            break;
        }
    }

    return splitted;
}

std::pair<std::string_view, std::string_view> split_str_2(std::string_view str, char delimiter) {
    if (auto pos = str.find(delimiter); pos != std::string_view::npos) {
        return std::make_pair(
            strip_str(str.substr(0, pos)),
            strip_str(str.substr(pos + 1, str.size() - pos + 2))
        );
    }
    return std::make_pair(str, "");
}

