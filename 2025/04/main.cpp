#include <math.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Point_t {
    int x;
    int y;
    static int y_max;
    static int x_max;
    bool inBounds() {
        if ((x >= x_max) || (x < 0) || (y >= y_max) || (y < 0)) {
            return false;
        }
        return true;
    }
    Point_t operator+(Point_t &p) {
        Point_t sum = {.x = this->x + p.x, .y = this->y + p.y};
        return sum;
    }
};

int Point_t::x_max = 0;
int Point_t::y_max = 0;

typedef std::vector<std::vector<bool>> Input;
static std::vector<Point_t> dir = {Point_t{1, 0},  Point_t{1, 1},   Point_t{0, 1},  Point_t{-1, 1},
                                   Point_t{-1, 0}, Point_t{-1, -1}, Point_t{0, -1}, Point_t{1, -1}};

Input parseInput(std::string fileName);

size_t greyStar(Input input);
size_t goldStar(Input input);

int removeRolls(Input *input);

int main(int, char *[]) {
    Input input = parseInput("input");
    std::cout << "res first star : " << greyStar(input) << std::endl;
    std::cout << "res second star : " << goldStar(input) << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    Input res;

    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                std::vector<bool> row;
                for (int i = 0; i < line.size(); ++i) {
                    if ('.' == line[i]) {
                        row.emplace_back(false);
                    } else {
                        row.emplace_back(true);
                    }
                }
                res.emplace_back(row);
            }
        }
    }
    Point_t::x_max = res[0].size();
    Point_t::y_max = res.size();
    std::cout << "x_max: " << Point_t::x_max << " y_max: " << Point_t::y_max << std::endl;
    return res;
}

size_t greyStar(Input input) { return removeRolls(&input); }

size_t goldStar(Input input) {
    size_t count = 0;
    int inter_total = 0;
    do {
        inter_total = removeRolls(&input);
        count += inter_total;
    } while (inter_total != 0);

    return count;
}

int removeRolls(Input *input) {
    int count = 0;
    int total = 0;
    Point_t current, near;
    Input output = *input;
    for (int y = 0; y < Point_t::y_max; y++) {
        for (int x = 0; x < Point_t::x_max; ++x) {
            count = 0;
            Point_t current = Point_t{.x = x, .y = y};
            if ((*input)[current.y][current.x]) {
                for (auto it = dir.begin(); it != dir.end(); it++) {
                    near = current + *it;
                    if (near.inBounds() && (*input)[near.y][near.x]) {
                        count++;
                    }
                }
                if (count < 4) {
                    total++;
                    output[current.y][current.x] = false;
                }
            }
        }
    }
    *input = output;
    return total;
}