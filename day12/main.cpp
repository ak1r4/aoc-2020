#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <chrono>
#include <cmath>

const double pi = 3.14159265358979323846;

using namespace std::chrono;

int main(int, char** argv) {
    std::fstream input(argv[1]);
    std::stringstream str_buf;
    str_buf << input.rdbuf();
    std::string str = str_buf.str();

    auto t0 = high_resolution_clock::now();

    auto manhatten_dist = [](double x, double y) {
        return std::abs(x) + std::abs(y);
    };

    int direction = 0;
    double x = 0.0;
    double y = 0.0;
    std::string_view s = str;
    while (!s.empty()) {
        auto endl = s.find('\n');
        char cmd = s.substr(0, 1)[0];
        auto dist = std::stoi(s.substr(1, endl - 1).data());

        switch (cmd) {
            case 'F': {
                    double radian = direction * pi / 180;
                    x += std::cos(radian) * dist;
                    y += std::sin(radian) * dist;
                    break;
                }
            case 'N': {
                    y += dist;
                    break;
                }
            case 'S': {
                    y -= dist;
                    break;
                }
            case 'E': {
                    x += dist;
                    break;
                }
            case 'W': {
                    x -= dist;
                    break;
                }
            case 'L': {
                    direction += dist;
                    break;
                }
            case 'R': {
                    direction -= dist;
                    break;
                }
        }
        s.remove_prefix(endl + 1);
    }

    auto part_1 = manhatten_dist(x, y);

    auto t1 = high_resolution_clock::now();

    std::cout << "Part 1: " << part_1
              << "(Lapse " << duration_cast<nanoseconds>(t1 - t0).count() << " ns)\n";

    auto t2 = high_resolution_clock::now();

    x = 0.0;
    y = 0.0;
    double ix = 10.0;
    double iy = 1.0;
    s = str;
    while (!s.empty()) {
        auto endl = s.find('\n');
        char cmd = s.substr(0, 1)[0];
        auto dist = std::stoi(s.substr(1, endl - 1).data());

        switch (cmd) {
            case 'F': {
                    x += ix * dist;
                    y += iy * dist;
                    break;
                }
            case 'N': {
                    iy += dist;
                    break;
                }
            case 'S': {
                    iy -= dist;
                    break;
                }
            case 'E': {
                    ix += dist;
                    break;
                }
            case 'W': {
                    ix -= dist;
                    break;
                }
            case 'L': {
                    double rad = dist * pi / 180;
                    double tmp_x = std::cos(rad) * ix - std::sin(rad) * iy;
                    iy = std::sin(rad) * ix + std::cos(rad) * iy;
                    ix = tmp_x;
                    break;
                }
            case 'R': {
                    double rad = (360 - dist) * pi / 180;
                    double tmp_x = std::cos(rad) * ix - std::sin(rad) * iy;
                    iy = std::sin(rad) * ix + std::cos(rad) * iy;
                    ix = tmp_x;
                    break;
                }
        }
        s.remove_prefix(endl + 1);
    }

    auto part_2 = manhatten_dist(x, y);
    auto t3 = high_resolution_clock::now();

    std::cout << "Part 2: " << part_2
              << "(Lapse " << duration_cast<nanoseconds>(t3 - t2).count() << " ns)\n";
    return 0;
}
