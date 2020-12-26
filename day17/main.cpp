#include "../utils.h"
#include <bits/stdc++.h>
using namespace std;

template<std::size_t ND>
using coord_t = array<int, ND>;

template<std::size_t ND>
using cubes_t = unordered_set<coord_t<ND>>;

namespace std
{
    template<typename T, size_t N>
    struct hash<array<T, N> >
    {
        typedef array<T, N> argument_type;
        typedef size_t result_type;

        result_type operator()(const argument_type& a) const
        {
            hash<T> hasher;
            result_type h = 0;
            for (result_type i = 0; i < N; ++i)
            {
                h = h * 31 + hasher(a[i]);
            }
            return h;
        }
    };
}

template<size_t ND>
ostream& operator<<(ostream& os, const cubes_t<ND>& cubes) {
    for (const auto& coord : cubes) {
        os << "[";
        for (const auto& c : coord) {
            os << c << ", ";
        }
        os << "]\n";
    }
    return os;
}

template<std::size_t ND>
struct ConwayCubes {
    void add_active_cube(coord_t<ND>&& coord) { this->active_cubes_.insert(move(coord)); }

    void run_cycle() {
        cubes_t<ND> flips;

        for (const auto& coord : this->active_cubes_) {
            auto n_active_neighbours = this->count_active_cubes(coord);
            if (n_active_neighbours != 2 && n_active_neighbours != 3) {
                flips.insert(coord);
            }
        }

        cubes_t<ND> inactive_cubes;
        for (const auto& coord : this->active_cubes_) {
            this->neighbours_gen_.reset();
            for (auto it = ++this->neighbours_gen_.begin(); it != this->neighbours_gen_.end(); ++it) {
                coord_t<ND> neighbour;
                for (std::size_t i = 0; i < ND; ++i) {
                    neighbour[i] = (*it)[i] + coord[i];
                }
                if (this->active_cubes_.count(neighbour) == 0) {
                    inactive_cubes.insert(std::move(neighbour));
                }
            }
        }

        for (const auto& coord : inactive_cubes) {
            if (this->count_active_cubes(coord) == 3) {
                flips.insert(coord);
            }
        }

        for (const auto& coord : flips) {
            if (auto it = this->active_cubes_.find(coord); it != this->active_cubes_.end()) {
                this->active_cubes_.erase(it);
            } else {
                this->active_cubes_.insert(coord);
            }
        }
    }

    std::size_t n_actives() const { return this->active_cubes_.size(); }

    void debug_print() const {
        cout << this->active_cubes_ << "\n";
    }
private:
    cubes_t<ND> active_cubes_ {};
    Product<int, 3, ND> neighbours_gen_ {{0, -1, 1}};

    std::size_t count_active_cubes(const coord_t<ND>& coord) {
        std::size_t count = 0;
        this->neighbours_gen_.reset();
        for (auto it = ++this->neighbours_gen_.begin(); it != this->neighbours_gen_.end(); ++it) {
            coord_t<ND> neighbour;
            for (std::size_t i = 0; i < ND; ++i) {
                neighbour[i] = (*it)[i] + coord[i];
            }
            if (this->active_cubes_.count(std::move(neighbour)) != 0) {
                ++count;
            }
        }
        return count;
    }
};


int main(int, char** argv) {

    auto part_1 = [&](string_view s) {
        ConwayCubes<3> conway_cubes;

        int x = 0;
        while (!s.empty()) {
            auto endl = s.find('\n');
            int y = 0;
            for (const auto& c : s.substr(0, endl)) {
                if (c == '#') {
                    conway_cubes.add_active_cube({x, y, 0});
                }
                ++y;
            }
            s.remove_prefix(endl + 1);
            ++x;
        }

        for (int i = 0; i < 6; ++i) {
            conway_cubes.run_cycle();
        }

        return conway_cubes.n_actives();
    };
    auto part_2 = [](string_view s) {
        ConwayCubes<4> conway_cubes;

        int x = 0;
        while (!s.empty()) {
            auto endl = s.find('\n');
            int y = 0;
            for (const auto& c : s.substr(0, endl)) {
                if (c == '#') {
                    conway_cubes.add_active_cube({x, y, 0, 0});
                }
                ++y;
            }
            s.remove_prefix(endl + 1);
            ++x;
        }

        for (int i = 0; i < 6; ++i) {
            conway_cubes.run_cycle();
        }

        return conway_cubes.n_actives();
    };
    run(part_1, argv[1], "Part 1");
    run(part_2, argv[1], "Part 2");
    return 0;
}
