#include <math.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

struct Point_t {
    double x;
    double y;

    double rectArea(const Point_t &p) const {
        return (std::abs(x - p.x) + 1) * (std::abs(y - p.y) + 1);
    }

    bool operator!=(const Point_t &p) const { return x != p.x || y != p.y; }
};

std::ostream &operator<<(std::ostream &os, const Point_t &obj) {
    os << "[" << obj.x << "," << obj.y << "]";
    return os;
}

typedef std::vector<Point_t> Input;

bool isEdgeInsideRectangle(Point_t p1, Point_t p2, Input &input);

Input parseInput(std::string fileName);
std::vector<Point_t> makePseudoPerimeter(Input &input);

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
    std::string number;
    Input res;

    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                Point_t point;
                std::stringstream ss(line);
                std::getline(ss, number, ',');
                point.x = atoi(number.c_str());
                std::getline(ss, number);
                point.y = atoi(number.c_str());
                res.emplace_back(point);
            }
        }
    }
    return res;
}

size_t greyStar(Input &input) {
    size_t biggest = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        for (size_t j = i + 1; j < input.size(); ++j) {
            size_t area = input[i].rectArea(input[j]);
            if (area > biggest) {
                biggest = area;
            }
        }
    }
    return biggest;
}

size_t goldStar(Input &input) {
    size_t biggest = 0;
    auto perimeter = makePseudoPerimeter(input);
    for (size_t i = 0; i < input.size(); ++i) {
        for (size_t j = i + 1; j < input.size(); ++j) {
            size_t area = input[i].rectArea(input[j]);
            if (area > biggest) {
                if (!isEdgeInsideRectangle(input[i], input[j], perimeter)) {
                    biggest = area;
                }
            }
        }
    }
    return biggest;
}

std::vector<Point_t> makePseudoPerimeter(Input &input) {
    std::vector<Point_t> res;
    for (size_t i = 0; i < input.size(); ++i) {
        Point_t p1 = input[i];
        Point_t p2 = input[(i + 1) % input.size()];
        Point_t pmid = Point_t{(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
        res.emplace_back(p1);
        res.emplace_back(pmid);
        res.emplace_back(p2);
    }
    return res;
}

bool isEdgeInsideRectangle(Point_t p1, Point_t p2, Input &input) {
    size_t x_max = std::max(p1.x, p2.x);
    size_t x_min = std::min(p1.x, p2.x);
    size_t y_max = std::max(p1.y, p2.y);
    size_t y_min = std::min(p1.y, p2.y);

    for (size_t i = 0; i < input.size(); ++i) {
        Point_t p = input[i];
        if (x_min < p.x && p.x < x_max && y_min < p.y && p.y < y_max) {
            return true;
        }
    }
    return false;
}