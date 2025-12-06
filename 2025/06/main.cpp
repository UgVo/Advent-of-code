#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Input {
    std::vector<std::vector<std::string>> columns;
    std::vector<char> operation;
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
    std::string elem;
    std::vector<std::string> raw_input;
    std::vector<std::string> section;
    Input res;

    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                raw_input.push_back(line);
            }
        }
    }
    section.resize(raw_input.size() - 1);
    for (int i = 0; i < raw_input[0].size(); ++i) {
        bool flag = true;
        for (int j = 0; j < raw_input.size() - 1; ++j) {
            if (' ' != raw_input[j][i]) {
                flag = false;
            }
            section[j].push_back(raw_input[j][i]);
        }

        if (flag) {
            for (int j = 0; j < raw_input.size() - 1; ++j) {
                section[j].pop_back();
            }
            res.columns.emplace_back(section);
            section.clear();
            section.resize(raw_input.size() - 1);
        }
    }
    res.columns.emplace_back(section);

    std::stringstream ss_line(raw_input.back());
    while (std::getline(ss_line, elem, ' ')) {
        if ('*' == elem[0] || '+' == elem[0]) {
            res.operation.push_back(elem[0]);
        }
    }
    return res;
}

size_t greyStar(Input &input) {
    size_t count = 0;
    for (auto it = input.columns.begin(); it != input.columns.end(); ++it) {
        char c_operator = input.operation[it - input.columns.begin()];
        if ('*' == c_operator) {
            size_t total = 1;
            for (auto it_num = it->begin(); it_num != it->end(); ++it_num) {
                total *= atoi(it_num->c_str());
            }
            count += total;
        } else if ('+' == c_operator) {
            size_t total = 0;
            for (auto it_num = it->begin(); it_num != it->end(); ++it_num) {
                total += atoi(it_num->c_str());
            }
            count += total;
        }
    }
    return count;
}

size_t goldStar(Input &input) {
    size_t count = 0;

    for (auto it = input.columns.begin(); it != input.columns.end(); ++it) {
        size_t total = 0;
        char c_operator = input.operation[it - input.columns.begin()];
        if ('*' == c_operator) {
            total = 1;
        }
        for (int i = 0; i < it->at(0).size(); ++i) {
            std::string val;
            for (auto it_num = it->begin(); it_num != it->end(); ++it_num) {
                val.push_back(it_num->at(i));
            }
            if ('*' == c_operator) {
                total *= atoi(val.c_str());
            } else {
                total += atoi(val.c_str());
            }
        }
        count += total;
    }

    return count;
}