#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Machine_t {
    std::vector<bool> light_indicators;
    std::vector<std::vector<int>> buttons;
    std::vector<int> voltage_req;
};

typedef std::vector<Machine_t> Input;

Input parseInput(std::string fileName);

// size_t greyStar(Input &input);
// size_t goldStar(Input &input);

int main(int, char *[]) {
    Input input = parseInput("input-test");
    for (auto &it : input) {
        for (auto light : it.light_indicators) {
            std::cout << (light ? "#" : ".");
        }
        std::cout << std::endl;
    }

    // std::cout << "res first star : " << greyStar(input) << std::endl;
    // std::cout << "res second star : " << goldStar(input) << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    std::regex reg_light("([.#]+)");
    std::regex reg_button("\\((([\\d,]*))\\)");
    std::regex reg_voltage("\\{([\\d,]*)\\}");
    Input res;

    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            std::cout << line << std::endl;
            if (!line.empty()) {
                Machine_t machine;
                std::smatch match;
                auto begin = line.cbegin();
                auto end = line.cend();

                std::regex_search(begin, end, match, reg_light);
                for (auto elem : match[1].str()) {
                    machine.light_indicators.emplace_back('#' == elem);
                }

                // while (std::regex_search(begin, end, match, reg_button)) {
                //     std::cout << match[1] << std::endl;
                //     begin = match[0].second;
                // }

                // while (std::regex_search(begin, end, match, reg_voltage)) {
                //     std::cout << match[1] << std::endl;
                //     begin = match[0].second;
                // }
                // find_begin = std::sregex_token_iterator(line.begin(),
                //                                         line.end(),
                //                                         reg_button);

                // for (auto it = find_begin; it !=
                // std::sregex_token_iterator();
                //      ++it) {
                //     std::cout << it->length() << " " << it->str() <<
                //     std::endl;
                // }

                // find_begin = std::sregex_token_iterator(
                //     line.begin(), line.end(), reg_voltage);

                // for (auto it = find_begin; it !=
                // std::sregex_token_iterator();
                //      ++it) {
                //     std::cout << it->str() << std::endl;
                // }
                res.emplace_back(machine);
            }
        }
    }
    return res;
}

// size_t greyStar(Input &input) {
//     size_t biggest = 0;
//     for (size_t i = 0; i < input.size(); ++i) {
//         for (size_t j = i + 1; j < input.size(); ++j) {
//             size_t area = input[i].rectArea(input[j]);
//             if (area > biggest) {
//                 biggest = area;
//             }
//         }
//     }
//     return biggest;
// }

// size_t goldStar(Input &input) {
//     size_t biggest = 0;
//     auto perimeter = makePseudoPerimeter(input);
//     for (size_t i = 0; i < input.size(); ++i) {
//         for (size_t j = i + 1; j < input.size(); ++j) {
//             size_t area = input[i].rectArea(input[j]);
//             if (area > biggest) {
//                 if (!isEdgeInsideRectangle(input[i], input[j], perimeter)) {
//                     biggest = area;
//                 }
//             }
//         }
//     }
//     return biggest;
// }

// std::vector<Point_t> makePseudoPerimeter(Input &input) {
//     std::vector<Point_t> res;
//     for (size_t i = 0; i < input.size(); ++i) {
//         Point_t p1 = input[i];
//         Point_t p2 = input[(i + 1) % input.size()];
//         Point_t pmid = Point_t{(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
//         res.emplace_back(p1);
//         res.emplace_back(pmid);
//         res.emplace_back(p2);
//     }
//     return res;
// }

// bool isEdgeInsideRectangle(Point_t p1, Point_t p2, Input &input) {
//     size_t x_max = std::max(p1.x, p2.x);
//     size_t x_min = std::min(p1.x, p2.x);
//     size_t y_max = std::max(p1.y, p2.y);
//     size_t y_min = std::min(p1.y, p2.y);

//     for (size_t i = 0; i < input.size(); ++i) {
//         Point_t p = input[i];
//         if (x_min < p.x && p.x < x_max && y_min < p.y && p.y < y_max) {
//             return true;
//         }
//     }
//     return false;
// }