#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Input {
    std::vector<std::pair<size_t, size_t>> ranges;
    std::vector<size_t> id;
};

Input parseInput(std::string fileName);
void compactInput(Input &input);

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
    int part = 0;
    int index = 0;
    Input res;

    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (line.empty()) {
                part = 1;
            }
            if (!line.empty()) {
                if (0 == part) {
                    index = line.find('-');
                    res.ranges.emplace_back(
                        std::make_pair(atoll(line.substr(0, index).c_str()),
                                       atoll(line.substr(index + 1, std::string::npos).c_str())));
                } else {
                    res.id.emplace_back(atoll(line.c_str()));
                }
            }
        }
    }
    return res;
}

size_t greyStar(Input &input) {
    size_t count = 0;
    for (auto it = input.id.begin(); it != input.id.end(); ++it) {
        for (auto range_it = input.ranges.begin(); range_it != input.ranges.end(); ++range_it) {
            if (*it <= range_it->second && *it >= range_it->first) {
                count++;
                break;
            }
        }
    }
    return count;
}

void compactInput(Input &input) {
    std::sort(
        input.ranges.begin(), input.ranges.end(),
        [](std::pair<size_t, size_t> a, std::pair<size_t, size_t> b) { return a.first < b.first; });
    for (size_t i = 1; i < input.ranges.size(); ++i) {
        if (input.ranges[i].first <= input.ranges[i - 1].second) {
            input.ranges[i - 1].second =
                std::max(input.ranges[i].second, input.ranges[i - 1].second);
            input.ranges.erase(input.ranges.begin() + i);
            i--;
        }
    }
}

size_t goldStar(Input &input) {
    size_t count = 0;
    compactInput(input);
    for (auto it = input.ranges.begin(); it != input.ranges.end(); ++it) {
        count += it->second - it->first + 1;
    }

    return count;
}