#include "../utils.h"
#include <bits/stdc++.h>
using namespace std;

using players_t = array<deque<int>, 2>;

pair<int, int> get_top_cards(players_t& players) {
    auto cards = make_pair(players[0].front(), players[1].front());
    players[0].pop_front();
    players[1].pop_front();
    return cards;
}

int calc_score(const deque<int>& player) {
    int multiplier = 1;
    int score = 0;
    for_each(player.rbegin(), player.rend(), [&](const auto& s) {
        score += multiplier * s;
        ++multiplier;
    });
    return score;
};

size_t hash_deck(const deque<int>& deck) {
	size_t s = deck.size();
    for (const auto& c : deck) {
        s ^= c + 0x9e3779b9 + (s << 6) + (s >> 2);
    }
    return s;
}

int run_game(const players_t& players_, int len_deck1, int len_deck2, int level) {
    array<unordered_set<size_t>, 2> seen_rounds;
    players_t players;
    players[0] = deque<int>(players_[0].begin(), players_[0].begin() + len_deck1);
    players[1] = deque<int>(players_[1].begin(), players_[1].begin() + len_deck2);

    auto play = [&]() -> int {
        while (true) {
            if (players[0].empty()) {
                return 1;
            } else if (players[1].empty()) {
                return 0;
            }

            auto not_seen_player1 = seen_rounds[0].insert(hash_deck(players[0])).second;
            auto not_seen_player2 = seen_rounds[1].insert(hash_deck(players[1])).second;
            if (!(not_seen_player1 && not_seen_player2)) {
                return 0;  // player 1 won;
            }

            auto [top1, top2] = get_top_cards(players);

            int winner_index = -1;

            if (players[0].size() >= top1 && players[1].size() >= top2) {  // play subgame
                winner_index = run_game(players, top1, top2, level + 1);
            } else {
                if (top1 > top2) {
                    winner_index = 0;
                } else {
                    winner_index = 1;
                }
            }

            if (winner_index == 0) {
                players[0].push_back(top1);
                players[0].push_back(top2);
            } else if (winner_index == 1){
                players[1].push_back(top2);
                players[1].push_back(top1);
            } else {
                throw;  // never gonna reach here
            }
        }
        throw;  // never gonna reach here
    };

    // calculate score when we are at the top level
    if (level == 0) {
        return calc_score(players[play()]);
    }
    return play();
}

int main(int, char** argv) {
    auto read_cards = [](string_view s, players_t& players) {
        skip_lines(s, 1);

        while (true) {
            auto endl = s.find('\n');
            if (endl == 0) break;
            players[0].push_back(stoi(s.substr(0, endl).data()));
            s.remove_prefix(endl + 1);
        }

        skip_lines(s, 2);

        while (!s.empty()) {
            auto endl = s.find('\n');
            players[1].push_back(stoi(s.substr(0, endl).data()));
            s.remove_prefix(endl + 1);
        }
    };

    auto part_1 = [&](string_view s) {
        players_t players;
        read_cards(s, players);

        while (true) {
            if (players[0].empty()) {
                return calc_score(players[1]);
            } else if (players[1].empty()) {
                return calc_score(players[0]);
            }

            auto [top1, top2] = get_top_cards(players);

            if (top1 > top2) {
                players[0].push_back(top1);
                players[0].push_back(top2);
            } else {
                players[1].push_back(top2);
                players[1].push_back(top1);
            }
        }

        return -1;
    };

    auto part_2 = [&](string_view s) {
        players_t players;
        read_cards(s, players);

        return run_game(players, players[0].size(), players[1].size(), 0);
    };

    run(part_1, argv[1], "Part 1");
    run(part_2, argv[1], "Part 2");
    return 0;
}
