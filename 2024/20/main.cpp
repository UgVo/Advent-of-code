#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#define THRESHOLD 100
// #define THRESHOLD 50
#define ALLOWED_DEPTH 20

struct Pos2D {
    int x;
    int y;

    Pos2D operator+(const Pos2D &other) const { return Pos2D{x + other.x, y + other.y}; }
    Pos2D operator-(const Pos2D &other) const { return Pos2D{x - other.x, y - other.y}; }
    int distanceTo(const Pos2D &other) const {
        return std::abs(other.x - x) + std::abs(other.y - y);
    }
    bool operator==(const Pos2D &other) const { return x == other.x && y == other.y; }

    bool inbound(int maxWidth, int maxHeight, int minWidth = 0, int minHeight = 0) const {
        return x >= minWidth && x < maxWidth && y >= minHeight && y < maxHeight;
    }

    bool inbound(Pos2D size) const { return x < size.x && y < size.y && x >= 0 && y >= 0; }

    bool orthogonal(const Pos2D &other) const {
        return (x == other.y || x == -other.y) && (y == other.x || y == -other.x);
    }
};

std::ostream &operator<<(std::ostream &os, Pos2D pos) {
    os << "[" << pos.x << "," << pos.y << "]";
    return os;
}
struct Pos2DCompare {
    bool operator()(const Pos2D &lhs, const Pos2D &rhs) const {
        return lhs.x * 1000 + lhs.y < rhs.x * 1000 + rhs.y;
    }
};

typedef std::map<Pos2D, int, Pos2DCompare> CostMap;
typedef std::set<Pos2D, Pos2DCompare> PosSet;

struct Input {
    std::vector<std::vector<bool>> map;
    Pos2D start;
    Pos2D end;
    Pos2D size;
};

Input parseInput(std::string fileName);
CostMap getCostMap(Input &input);
std::vector<std::pair<Pos2D, Pos2D>> getShortcuts(Input &input, CostMap &costMap);
PosSet availableShortcutsFrom(Pos2D start, int dephAllowed, const Input &input);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char *[]) {
    auto input = parseInput("input");
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "res gray star : " << grayStar(input) << std::endl;
    std::cout << "res gold star : " << goldStar(input) << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
              << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input res;
    std::string line;

    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                std::vector<bool> tmp;
                for (int i = 0; i < line.size(); ++i) {
                    if (line[i] == '#') {
                        tmp.push_back(false);
                    } else if (line[i] == '.') {
                        tmp.push_back(true);
                    } else if (line[i] == 'S') {
                        res.start = Pos2D{int(tmp.size()), int(res.map.size())};
                        tmp.push_back(true);
                    } else if (line[i] == 'E') {
                        res.end = Pos2D{int(tmp.size()), int(res.map.size())};
                        tmp.push_back(true);
                    }
                }
                res.map.push_back(tmp);
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    res.size = Pos2D{int(res.map[0].size()), int(res.map.size())};
    return res;
}

CostMap getCostMap(Input &input) {
    CostMap res;
    std::vector<Pos2D> dirs{Pos2D{0, 1}, Pos2D{1, 0}, Pos2D{0, -1}, Pos2D{-1, 0}};
    Pos2D current = input.start;
    auto mapCopy = input.map;
    int cost = 0;
    while (current != input.end) {
        for (auto dir : dirs) {
            res.insert(std::make_pair(current, cost));
            mapCopy[current.y][current.x] = false;
            Pos2D newPos = current + dir;
            if (newPos.inbound(input.size) && mapCopy[newPos.y][newPos.x]) {
                current = newPos;
                cost++;
            }
        }
    }
    return res;
}

std::vector<std::pair<Pos2D, Pos2D>> getShortcuts(Input &input, CostMap &costMap) {
    std::vector<Pos2D> dirs{Pos2D{0, 1}, Pos2D{1, 0}, Pos2D{0, -1}, Pos2D{-1, 0}};
    std::vector<std::pair<Pos2D, Pos2D>> res;
    for (auto costPair : costMap) {
        for (auto dir : dirs) {
            Pos2D wall = costPair.first + dir;
            Pos2D track = wall + dir;
            if (wall.inbound(input.size) && track.inbound(input.size) &&
                !input.map[wall.y][wall.x] && input.map[track.y][track.x] &&
                costMap.contains(track)) {
                res.push_back(std::make_pair(costPair.first, track));
            }
        }
    }

    return res;
}

std::vector<std::pair<Pos2D, Pos2D>> getShortcuts(Input &input, CostMap &costMap, int dephAllowed) {
    std::vector<std::pair<Pos2D, Pos2D>> res;
    for (auto src : costMap) {
        for (auto dst : costMap) {
            if (src.first != dst.first && src.first.distanceTo(dst.first) <= dephAllowed) {
                res.push_back(std::make_pair(src.first, dst.first));
            }
        }
    }
    return res;
}

size_t grayStar(Input input) {
    size_t res = 0;
    auto costMap = getCostMap(input);
    auto shortcuts = getShortcuts(input, costMap);
    for (auto shortcut : shortcuts) {
        if (costMap[shortcut.second] - costMap[shortcut.first] -
                shortcut.first.distanceTo(shortcut.second) >=
            THRESHOLD) {
            res += 1;
        }
    }
    return res;
}

size_t goldStar(Input input) {
    size_t res = 0;
    auto costMap = getCostMap(input);
    auto shortcuts = getShortcuts(input, costMap, ALLOWED_DEPTH);

    for (auto shortcut : shortcuts) {
        Pos2D src, dst;
        src = shortcut.first;
        dst = shortcut.second;
        int gain = costMap[shortcut.second] - costMap[shortcut.first] - src.distanceTo(dst);

        if (gain >= THRESHOLD) {
            res += 1;
        }
    }

    return res;
}