#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::pair<char, int>> Input;

Input parseInput(std::string fileName);

size_t greyStar(Input input);
size_t goldStar(Input input);

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
                res.emplace_back(std::make_pair(line[0], atoi(line.substr(1).c_str())));
            }
        }
    }
    return res;
}

size_t greyStar(Input input) {
    int dial = 50;
    int count = 0;
    for (auto it = input.begin(); it != input.end(); ++it) {
        if (it->first == 'R') {
            dial += it->second % 100;
            if (dial > 99) {
                dial = dial - 100;
            }
        } else {
            dial -= it->second % 100;
            if (dial < 0) {
                dial = dial + 100;
            }
        }
        if (dial == 0) {
            count++;
        }
    }

    return count;
}

size_t goldStar(Input input) {
    int dial = 50;
    int count = 0;
    int round_count = 0;
    for (auto it = input.begin(); it != input.end(); ++it) {
        if (it->first == 'R') {
            dial += it->second;
            if (dial > 99) {
                round_count = std::abs(dial / 100);
                dial = dial % 100;
                count += round_count;
            }
        } else {
            bool flag = (dial != 0);
            dial -= it->second;
            if (dial < 0) {
                round_count = std::abs(dial / 100) + flag;
                dial = dial % 100 + 100;
                count += round_count;
            }
            if (dial == 100) {
                dial = 0;
            }
            if (dial == 0 && round_count == 0) {
                count++;
            }
        }
        round_count = 0;
    }
    return count;
}