#include <math.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<std::pair<long, long>> Input;

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
    int index = 0;
    Input res;
    std::string value;

    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            std::stringstream ss(line);
            if (!line.empty()) {
                while (getline(ss, value, ',')) {
                    index = value.find('-');
                    res.emplace_back(
                        std::make_pair(atol(value.substr(0, index).c_str()),
                                       atol(value.substr(index + 1, std::string::npos).c_str())));
                }
            }
        }
    }
    return res;
}

size_t greyStar(Input input) {
    size_t count = 0;
    size_t index = 0;
    int num_size = 0;
    int mid_power = 0;
    for (auto it = input.begin(); it != input.end(); ++it) {
        for (index = it->first; index <= it->second; ++index) {
            num_size = std::log10((long double)index) + 1;
            if (num_size % 2 == 0) {
                mid_power = int(std::pow(10, num_size / 2));
                if (index % mid_power == index / mid_power) {
                    count += index;
                }
            }
        }
    }

    return count;
}

size_t goldStar(Input input) {
    size_t count = 0;
    size_t index = 0;
    int num_size = 0;
    int power_low = 0;
    int power = 0;
    int current = 0;
    bool flag = false;
    bool counted = false;
    std::vector<int> primes = {2, 3, 5, 7};  // no need to go further up the prime numbers as the
                                             // max number of digit in the input is 10
    for (auto it = input.begin(); it != input.end(); it++) {
        for (index = it->first; index <= it->second; index++) {
            flag = false;
            counted = false;
            num_size = std::log10((long double)index) + 1;
            for (auto prime_num = primes.begin(); prime_num != primes.end(); ++prime_num) {
                if (num_size % *prime_num == 0) {
                    flag = true;
                    power_low = int(std::pow(10, num_size / *prime_num));
                    current = index % power_low;
                    for (int i = 1; i < *prime_num; ++i) {
                        power = int(std::pow(10, (num_size / *prime_num) * i));
                        flag = (current == (index / power) % power_low) && flag;
                    }
                    if (flag) {
                        count += index;
                        break;
                    }
                }
            }
        }
    }

    return count;
}