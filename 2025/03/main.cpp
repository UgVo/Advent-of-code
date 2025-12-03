#include <math.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::vector<int>> Input;

Input parseInput(std::string fileName);

size_t greyStar(Input input);
size_t goldStar(Input input);

size_t toNumeral(std::vector<int> vect);
size_t getGreatest(std::vector<int> array, int number_of_digits);

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
                std::vector<int> bank;
                for (int i = 0; i < line.size(); ++i) {
                    if ('0' <= line[i] && '9' >= line[i]) {
                        bank.emplace_back(line[i] - '0');
                    }
                }
                res.emplace_back(bank);
            }
        }
    }
    return res;
}

size_t greyStar(Input input) {
    size_t count = 0;
    for (auto it = input.begin(); it != input.end(); ++it) {
        count += getGreatest(*it, 2);
    }
    return count;
}

size_t goldStar(Input input) {
    size_t count = 0;
    for (auto it = input.begin(); it != input.end(); ++it) {
        count += getGreatest(*it, 12);
    }
    return count;
}

size_t toNumeral(std::vector<int> vect) {
    size_t numeral = 0;
    for (int i = 0; i < vect.size(); i++) {
        numeral += vect[i] * std::pow(10, vect.size() - 1 - i);
    }
    return numeral;
}

size_t getGreatest(std::vector<int> array, int number_of_digits) {
    std::vector<int> array_cpy = array;
    std::vector<int> number_vect;
    for (int i = 0; i < number_of_digits; ++i) {
        int upper_bound = array_cpy.size() - (number_of_digits - number_vect.size()) + 1;
        auto greatest_it = std::max_element(array_cpy.begin(), array_cpy.begin() + upper_bound);
        number_vect.push_back(*greatest_it);
        array_cpy.erase(array_cpy.begin(), greatest_it + 1);
    }
    return toNumeral(number_vect);
}