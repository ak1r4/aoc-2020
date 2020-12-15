#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <string_view>
#include <cstdint>
#include <vector>
#include <set>
#include <deque>
#include <chrono>

template<typename T>
struct PrevNums {
    explicit PrevNums(std::size_t size) : size_(size) {}

    void push(const T& num) {
        if (this->queue_.size() >= this->size_) {
            this->set_.erase(this->queue_.front());
            this->queue_.pop_front();
        }
        this->queue_.push_back(num);
        this->set_.insert(num);
    }

    bool is_valid_sum(const T& num) const {
        for (const auto& n : this->set_) {
            if (auto it = this->set_.find(num - n); it != this->set_.end() && *it != n) {
                return true;
            }
        }
        return false;
    }

private:
    const std::size_t size_;
    std::set<T> set_;
    std::deque<T> queue_;
};

template<typename T>
struct SumMinMaxQueue {
    explicit SumMinMaxQueue(const T& target_sum) : target_sum_(target_sum), sum_so_far_(0) {}

    bool push(const T& num) {
        // Pop until sum so far is not bigger than target sum
        while (this->sum_so_far_ > this->target_sum_) {
            const auto& item_to_be_popped = this->queue_.front();
            if (item_to_be_popped == this->max_queue_.front()) {
                this->max_queue_.pop_front();
            }
            if (item_to_be_popped == this->min_queue_.front()) {
                this->min_queue_.pop_front();
            }
            this->queue_.pop_front();
            this->sum_so_far_ -= item_to_be_popped;
        }

        if (this->sum_so_far_ < this->target_sum_) {
            this->queue_.push_back(num);
            while (!this->max_queue_.empty() && this->max_queue_.back() < num) {
                this->max_queue_.pop_back();
            }
            this->max_queue_.push_back(num);
            while(!this->min_queue_.empty() && this->min_queue_.back() > num) {
                this->min_queue_.pop_back();
            }
            this->min_queue_.push_back(num);
            this->sum_so_far_ += num;
        }

        return this->sum_so_far_ == this->target_sum_;
    }

    const T& max() const {
        return this->max_queue_.front();
    }

    const T& min() const {
        return this->min_queue_.front();
    }
private:
    const T target_sum_;
    T sum_so_far_;
    std::deque<T> queue_;
    std::deque<T> max_queue_;
    std::deque<T> min_queue_;
};

const std::size_t N_PREAMBLE = 25;

int main() {
    std::fstream input("input.txt");
    std::stringstream input_buffer;
    input_buffer << input.rdbuf();
    std::string input_str = input_buffer.str();

    auto t0 = std::chrono::high_resolution_clock::now();
    std::string_view str_view = input_str;
    PrevNums<std::int64_t> prev_nums(N_PREAMBLE);
    std::vector<std::int64_t> nums;

    int idx = 0;
    std::int64_t invalid_num = 0;

    while (!str_view.empty()) {
        auto endl = str_view.find('\n');
        auto line = str_view.substr(0, endl);
        auto n = static_cast<std::int64_t>(std::stoll(line.data()));
        if (idx >= N_PREAMBLE && !prev_nums.is_valid_sum(n)) {
            invalid_num = n;
            break;
        }
        prev_nums.push(n);
        nums.push_back(n);
        str_view.remove_prefix(endl + 1);
        ++idx;
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Part 1: " << invalid_num;
    std::cout << " (Lapse: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() << " ns)\n";

    auto t2 = std::chrono::high_resolution_clock::now();
    SumMinMaxQueue<std::int64_t> sum_min_max_queue(invalid_num);
    std::int64_t minmax_sum = 0;

    for (const auto& n : nums) {
        if (sum_min_max_queue.push(n)) {
            minmax_sum = sum_min_max_queue.min() + sum_min_max_queue.max();
            break;
        }
    }

    auto t3 = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2: " << minmax_sum;
    std::cout << " (Lapse: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count() << " ns)\n";



    return 0;
}
