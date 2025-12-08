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
    long x;
    long y;
    long z;

    double distance(const Point_t &p) const {
        return std::sqrt(std::pow(x - p.x, 2) + std::pow(y - p.y, 2) +
                         std::pow(z - p.z, 2));
    }
};

std::ostream &operator<<(std::ostream &os, const Point_t &obj) {
    os << "[" << obj.x << "," << obj.y << "," << obj.z << "]";
    return os;
}

struct JboxPair_t {
    size_t first;
    size_t second;
    double distance;
};

struct Jbox_t {
    Point_t pos;
    int circuitId;
};

typedef std::vector<Jbox_t> Input;

struct Circuit_t {
    std::set<size_t> jboxIds;
    int id;
};

Input parseInput(std::string fileName);

std::vector<JboxPair_t> computeDistance(Input &input);
std::map<int, Circuit_t> computeCircuit(Input &input,
                                        std::vector<JboxPair_t> &pairs,
                                        int num_connections);
size_t computeCircuitMax(Input &input, std::vector<JboxPair_t> &pairs);

size_t greyStar(std::map<int, Circuit_t> &input);
size_t goldStar(Input &input, std::vector<JboxPair_t> &pairs);

int main(int, char *[]) {
    Input input = parseInput("input");
    auto pairs = computeDistance(input);

    auto circuits = computeCircuit(input, pairs, 1000);
    std::cout << "res first star : " << greyStar(circuits) << std::endl;
    std::cout << "res second star : " << goldStar(input, pairs) << std::endl;
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
                std::getline(ss, number, ',');
                point.y = atoi(number.c_str());
                std::getline(ss, number);
                point.z = atoi(number.c_str());
                res.emplace_back(Jbox_t{.pos = point, .circuitId = -1});
            }
        }
    }
    return res;
}

std::vector<JboxPair_t> computeDistance(Input &input) {
    std::vector<JboxPair_t> res;
    for (size_t i = 0; i < input.size(); ++i) {
        for (size_t j = i + 1; j < input.size(); ++j) {
            if (i != j) {
                res.emplace_back(
                    JboxPair_t{i, j, input[i].pos.distance(input[j].pos)});
            }
        }
    }
    std::sort(res.begin(), res.end(), [](JboxPair_t a, JboxPair_t b) {
        return a.distance < b.distance;
    });
    return res;
}

std::map<int, Circuit_t> computeCircuit(Input &input,
                                        std::vector<JboxPair_t> &pairs,
                                        int num_connections) {
    std::map<int, Circuit_t> res;
    Input input_cpy = input;
    int currentCircuitId = 0;
    int i = 0;

    while (i < num_connections && i < pairs.size()) {
        auto pair = pairs[i++];
        bool flag = false;
        if (-1 == input_cpy[pair.first].circuitId) {
            Circuit_t circuit;
            circuit.id = currentCircuitId++;

            circuit.jboxIds.insert(pair.first);
            input_cpy[pair.first].circuitId = circuit.id;

            res.insert(std::make_pair(circuit.id, circuit));
        }

        if (-1 == input_cpy[pair.second].circuitId) {
            input_cpy[pair.second].circuitId = input_cpy[pair.first].circuitId;
            res[input_cpy[pair.first].circuitId].jboxIds.insert(pair.second);
        } else if (input_cpy[pair.second].circuitId !=
                   input_cpy[pair.first].circuitId) {
            auto index_res = input_cpy[pair.second].circuitId;
            auto old_circuit = res[index_res].jboxIds;
            for (auto it = old_circuit.begin(); it != old_circuit.end(); ++it) {
                input_cpy[*it].circuitId = input_cpy[pair.first].circuitId;
                res[input_cpy[pair.first].circuitId].jboxIds.insert(*it);
            }
            res.erase(index_res);
        }
    }
    return res;
}

size_t computeCircuitMax(Input &input, std::vector<JboxPair_t> &pairs) {
    std::map<int, Circuit_t> res;
    int currentCircuitId = 0;
    int i = 0;

    while (res.begin()->second.jboxIds.size() != input.size() &&
           i < pairs.size()) {
        auto pair = pairs[i++];
        bool flag = false;
        if (-1 == input[pair.first].circuitId) {
            Circuit_t circuit;
            circuit.id = currentCircuitId++;

            circuit.jboxIds.insert(pair.first);
            input[pair.first].circuitId = circuit.id;

            res.insert(std::make_pair(circuit.id, circuit));
        }

        if (-1 == input[pair.second].circuitId) {
            input[pair.second].circuitId = input[pair.first].circuitId;
            res[input[pair.first].circuitId].jboxIds.insert(pair.second);
        } else if (input[pair.second].circuitId !=
                   input[pair.first].circuitId) {
            auto index_res = input[pair.second].circuitId;
            auto old_circuit = res[index_res].jboxIds;
            for (auto it = old_circuit.begin(); it != old_circuit.end(); ++it) {
                input[*it].circuitId = input[pair.first].circuitId;
                res[input[pair.first].circuitId].jboxIds.insert(*it);
            }
            res.erase(index_res);
        }
    }

    return i - 1;
}

size_t greyStar(std::map<int, Circuit_t> &circuits) {
    std::vector<size_t> sizes;
    for (auto it = circuits.begin(); it != circuits.end(); ++it) {
        sizes.push_back(it->second.jboxIds.size());
    }
    std::sort(sizes.begin(), sizes.end(), std::greater<int>());

    return sizes[0] * sizes[1] * sizes[2];
}

size_t goldStar(Input &input, std::vector<JboxPair_t> &pairs) {
    size_t index = computeCircuitMax(input, pairs);
    return input[pairs[index].first].pos.x * input[pairs[index].second].pos.x;
}