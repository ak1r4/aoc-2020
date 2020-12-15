#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <chrono>
#include <vector>
#include <cstdint>
#include <utility>

using namespace std::chrono;

enum class State : std::uint8_t {
    FLOOR = 0,
    EMPTY = 1,
    OCCUPIED = 2,
};

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<State>>& layout) {
    for (const auto& row : layout) {
        for (const auto& i : row) {
            switch (i) {
                case State::FLOOR:
                    os << ".";
                    break;
                case State::EMPTY:
                    os << "L";
                    break;
                case State::OCCUPIED:
                    os << "#";
                    break;
            }
        }
        os << "\n";
    }
    return os;
}

template<std::size_t PART_N, bool DEBUG>
std::size_t evolve(std::vector<std::vector<State>> seatings) {
    std::size_t h = seatings.size();
    std::size_t w = seatings[0].size();

    std::vector<std::pair<std::size_t, std::size_t>> seatings_should_flip;

    auto should_occupy_1 = [&](std::size_t x, std::size_t y) -> bool {
        if (y > 0 && x > 0 && seatings[x - 1][y - 1] == State::OCCUPIED) return false;
        if (x > 0 && seatings[x - 1][y] == State::OCCUPIED) return false;
        if (x > 0 && y < w - 1 && seatings[x - 1][y + 1] == State::OCCUPIED) return false;
        if (y > 0 && seatings[x][y - 1] == State::OCCUPIED) return false;
        if (y < w - 1 && seatings[x][y + 1] == State::OCCUPIED) return false;
        if (y > 0 && x < h - 1 && seatings[x + 1][y - 1] == State::OCCUPIED) return false;
        if (x < h - 1 && seatings[x + 1][y] == State::OCCUPIED) return false;
        if (x < h - 1 && y < w - 1 && seatings[x + 1][y + 1] == State::OCCUPIED) return false;
        return true;
    };
    auto should_empty_1 = [&](std::size_t x, std::size_t y) -> bool {
        std::size_t n_occupied = 0;
        if (y > 0 && x > 0 && seatings[x - 1][y - 1] == State::OCCUPIED) ++n_occupied;
        if (x > 0 && seatings[x - 1][y] == State::OCCUPIED) ++n_occupied;
        if (x > 0 && y < w - 1 && seatings[x - 1][y + 1] == State::OCCUPIED) ++n_occupied;
        if (y > 0 && seatings[x][y - 1] == State::OCCUPIED) ++n_occupied;
        if (y < w - 1 && seatings[x][y + 1] == State::OCCUPIED) ++n_occupied;
        if (y > 0 && x < h - 1 && seatings[x + 1][y - 1] == State::OCCUPIED) ++n_occupied;
        if (x < h - 1 && seatings[x + 1][y] == State::OCCUPIED) ++n_occupied;
        if (x < h - 1 && y < w - 1 && seatings[x + 1][y + 1] == State::OCCUPIED) ++n_occupied;
        if (n_occupied >= 4) return true;
        return false;
    };

    auto upper_left_occupied = [&](std::size_t x, std::size_t y) -> bool {
        if (x == 0 || y == 0) {
            return false;
        }
        do {
            --x;
            --y;
            if (seatings[x][y] != State::FLOOR) return seatings[x][y] == State::OCCUPIED ? true: false;
        } while (x > 0 && y > 0);
        return false;
    };
    auto upper_mid_occupied = [&](std::size_t x, std::size_t y) -> bool {
        if (x == 0) {
            return false;
        }

        do {
            --x;
            if (seatings[x][y] != State::FLOOR) return seatings[x][y] == State::OCCUPIED ? true: false;
        } while (x > 0);
        return false;
    };
    auto upper_right_occupied = [&](std::size_t x, std::size_t y) -> bool {
        if (x == 0 || y == w - 1) {
            return false;
        }

        do {
            --x;
            ++y;
            if (seatings[x][y] != State::FLOOR) return seatings[x][y] == State::OCCUPIED ? true: false;
        } while (x > 0 && y < w - 1);
        return false;
    };
    auto left_occupied = [&](std::size_t x, std::size_t y) -> bool {
        if (y == 0) {
            return false;
        }

        do {
            --y;
            if (seatings[x][y] != State::FLOOR) return seatings[x][y] == State::OCCUPIED ? true: false;
        } while (y > 0);
        return false;
    };
    auto right_occupied = [&](std::size_t x, std::size_t y) -> bool {
        if (y == w - 1) {
            return false;
        }
        do {
            ++y;
            if (seatings[x][y] != State::FLOOR) return seatings[x][y] == State::OCCUPIED ? true: false;
        } while (y < w - 1);
        return false;
    };
    auto lower_left_occupied = [&](std::size_t x, std::size_t y) -> bool {
        if (x == h - 1 || y == 0) {
            return false;
        }
        do {
            ++x;
            --y;
            if (seatings[x][y] != State::FLOOR) return seatings[x][y] == State::OCCUPIED ? true: false;
        } while (x < h - 1 && y > 0);
        return false;
    };
    auto lower_mid_occupied = [&](std::size_t x, std::size_t y) -> bool {
        if (x == h - 1) {
            return false;
        }
        do {
            ++x;
            if (seatings[x][y] != State::FLOOR) return seatings[x][y] == State::OCCUPIED ? true: false;
        } while (x < h - 1);
        return false;
    };
    auto lower_right_occupied = [&](std::size_t x, std::size_t y) -> bool {
        if (x == h - 1 || y == w - 1) {
            return false;
        }
        do {
            ++x;
            ++y;
            if (seatings[x][y] != State::FLOOR) return seatings[x][y] == State::OCCUPIED ? true: false;
        } while (x < h - 1 && y < w - 1);
        return false;
    };

    auto should_occupy_2 = [&](std::size_t x, std::size_t y) -> bool {
        if (upper_left_occupied(x, y)) return false;
        if (upper_mid_occupied(x, y)) return false;
        if (upper_right_occupied(x, y)) return false;
        if (left_occupied(x, y)) return false;
        if (right_occupied(x, y)) return false;
        if (lower_left_occupied(x, y)) return false;
        if (lower_mid_occupied(x, y)) return false;
        if (lower_right_occupied(x, y)) return false;
        return true;
    };


    auto should_empty_2 = [&](std::size_t x, std::size_t y) -> bool {
        std::size_t n_occupied = 0;
        if (upper_left_occupied(x, y)) ++n_occupied;
        if (upper_mid_occupied(x, y)) ++n_occupied;
        if (upper_right_occupied(x, y)) ++n_occupied;
        if (left_occupied(x, y)) ++n_occupied;
        if (right_occupied(x, y)) ++n_occupied;
        if (lower_left_occupied(x, y)) ++n_occupied;
        if (lower_mid_occupied(x, y)) ++n_occupied;
        if (lower_right_occupied(x, y)) ++n_occupied;
        if (n_occupied >= 5) return true;
        return false;
    };

    int iterations = 0;
    do {
        if constexpr (DEBUG) {
            std::cout << iterations << "\n";
            std::cout << seatings << "\n";
        }

        seatings_should_flip.clear();

        for (std::size_t i = 0; i < h; ++i) {
            for (std::size_t j = 0; j < w; ++j) {
                const auto& cur = seatings[i][j];
                if (cur == State::FLOOR) {
                    continue;
                }


                if (cur == State::EMPTY) {
                    if constexpr (PART_N == 1) {
                        if (should_occupy_1(i, j)) seatings_should_flip.push_back(std::make_pair(i, j));
                    } else if (PART_N == 2) {
                        if (should_occupy_2(i, j)) seatings_should_flip.push_back(std::make_pair(i, j));
                    }
                }
                else if (cur == State::OCCUPIED) {
                    if constexpr (PART_N == 1) {
                        if (should_empty_1(i, j)) seatings_should_flip.push_back(std::make_pair(i, j));
                    } else if (PART_N == 2) {
                        if (should_empty_2(i, j)) seatings_should_flip.push_back(std::make_pair(i, j));
                    }
                }
            }
        }

        for (const auto& [i, j] : seatings_should_flip) {
            switch (seatings[i][j]) {
                case State::EMPTY:
                    seatings[i][j] = State::OCCUPIED;
                    break;
                case State::OCCUPIED:
                    seatings[i][j] = State::EMPTY;
                    break;
            }
        }
        ++iterations;
    } while (!seatings_should_flip.empty());

    std::size_t n_occupied = 0;
    for (const auto& row : seatings) {
        for (const auto& c : row) {
            if (c == State::OCCUPIED) ++n_occupied;
        }
    }
    return n_occupied;
}

int main(int, char** argv) {
    std::fstream input(argv[1]);
    std::stringstream str_buf;
    str_buf << input.rdbuf();
    std::string str = str_buf.str();

    auto t0 = high_resolution_clock::now();
    std::string_view s = str;
    std::vector<std::vector<State>> seatings;

    while (!s.empty()) {
        auto endl = s.find('\n');
        std::vector<State> row;
        for (const auto& c : s.substr(0, endl)) {
            switch (c) {
                case '.':
                    row.push_back(State::FLOOR);
                    break;
                case 'L':
                    row.push_back(State::EMPTY);
                    break;
                case '#':
                    row.push_back(State::OCCUPIED);
                    break;
            }
        }
        seatings.push_back(row);
        s.remove_prefix(endl + 1);
    }

    auto n_occupied = evolve<1, false>(seatings);

    auto t1 = high_resolution_clock::now();
    std::cout << "Part 1: " << n_occupied
              << "(Lapse " << duration_cast<nanoseconds>(t1 - t0).count() << " ns)\n";


    auto t2 = high_resolution_clock::now();
    n_occupied = evolve<2, false>(seatings);

    auto t3 = high_resolution_clock::now();
    std::cout << "Part 2: " << n_occupied
              << "(Lapse " << duration_cast<nanoseconds>(t3- t2).count() << " ns)\n";
}
