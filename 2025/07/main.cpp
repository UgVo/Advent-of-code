#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
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
    Point_t operator+(const Point_t &p) const {
        Point_t sum = {.x = this->x + p.x, .y = this->y + p.y};
        return sum;
    }

    bool operator==(const Point_t &p) const { return x == x && y == y; }
    std::size_t hash() const { return y * Point_t::y_max + x; }
};

int Point_t::x_max = 0;
int Point_t::y_max = 0;

Point_t down = {.x = 0, .y = 1};
Point_t left = {.x = -1, .y = 0};
Point_t right = {.x = 1, .y = 0};

struct Timeline_t {
    std::vector<Point_t> sources;
    Point_t current_pos;
};

struct Input {
    enum type_cell { space, splitter, processed };

    std::vector<std::vector<type_cell>> array;
    Point_t start;
};

Input parseInput(std::string fileName);

size_t greyStar(Input &input);
size_t goldStar(Input &input);

int main(int, char *[]) {
    Input input = parseInput("input");
    std::cout << "res first star : " << greyStar(input) << std::endl;
    std::cout << "res second star : " << goldStar(input) << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    std::vector<Input::type_cell> row;
    Input res;

    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                row.clear();
                for (auto it = line.begin(); it != line.end(); ++it) {
                    switch (*it) {
                        case 'S':
                            res.start = Point_t{.x = int(it - line.begin()), .y = 0};
                        case '.':
                            row.push_back(Input::space);
                            break;

                        case '^':
                            row.push_back(Input::splitter);
                            break;
                        default:
                            break;
                    }
                }
                res.array.emplace_back(row);
            }
        }
    }
    Point_t::x_max = res.array[0].size();
    Point_t::y_max = res.array.size();

    return res;
}

size_t greyStar(Input &input) {
    size_t count = 0;
    std::list<Point_t> pile;
    auto array_cpy = input.array;
    pile.push_back(input.start);
    while (!pile.empty()) {
        Point_t current = pile.front();
        pile.pop_front();
        Point_t next = current + down;
        if (next.inBounds()) {
            if (Input::space == array_cpy[next.y][next.x]) {
                pile.push_back(next);
            } else if (Input::splitter == array_cpy[next.y][next.x]) {
                Point_t split = next + right;
                if (Input::processed != array_cpy[split.y][split.x]) {
                    pile.push_back(split);
                    array_cpy[split.y][split.x] = Input::processed;
                }
                split = next + left;
                if (Input::processed != array_cpy[split.y][split.x]) {
                    pile.push_back(split);
                    array_cpy[split.y][split.x] = Input::processed;
                }
                array_cpy[next.y][next.x] = Input::processed;
                count++;
            }
        }
    }
    return count;
}

size_t goldStar(Input &input) {
    size_t count = 1;
    std::list<Timeline_t> pile;
    Timeline_t origin = {.sources = {input.start}, .current_pos = input.start};
    std::map<size_t, size_t> nodes_count;

    auto array_cpy = input.array;
    pile.push_back(origin);
    nodes_count.insert(std::make_pair(origin.current_pos.hash(), 0));
    while (!pile.empty()) {
        Timeline_t current = pile.back();
        pile.pop_back();

        Point_t next = current.current_pos + down;
        if (next.inBounds()) {
            if (Input::space == input.array[next.y][next.x]) {
                current.current_pos = next;
                pile.push_back(current);

            } else if (Input::splitter == input.array[next.y][next.x]) {
                if (!nodes_count.contains(next.hash())) {
                    nodes_count.insert(std::make_pair(next.hash(), 0));
                    current.sources.push_back(next);

                    pile.push_back(
                        Timeline_t{.sources = current.sources, .current_pos = next + right});
                    pile.push_back(
                        Timeline_t{.sources = current.sources, .current_pos = next + left});
                } else {
                    for (auto it = current.sources.begin(); it != current.sources.end(); ++it) {
                        nodes_count.at(it->hash()) += nodes_count.at(next.hash());
                    }
                }
            }
        } else {
            for (auto it = current.sources.begin(); it != current.sources.end(); ++it) {
                nodes_count.at(it->hash()) += 1;
            }
        }
    }
    count = nodes_count.at(origin.current_pos.hash());
    return count;
}