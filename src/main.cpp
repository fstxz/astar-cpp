#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <ostream>
#include <queue>
#include <unordered_map>
#include <vector>

struct Vec2i {
    int x, y;

    bool operator==(const Vec2i &other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
template <> struct hash<Vec2i> {
    size_t operator()(const Vec2i &p) const {
        return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
    }
};
} // namespace std

int graph[16][16] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

struct Pair {
    Vec2i position;
    float priority;

    bool operator<(const Pair &other) const {
        return priority > other.priority;
    }
};

std::vector<Vec2i> construct_path(std::unordered_map<Vec2i, Vec2i> came_from,
                                  Vec2i current) {
    std::vector<Vec2i> total_path;
    total_path.push_back(current);

    while (came_from.find(current) != came_from.end()) {
        current = came_from[current];
        total_path.push_back(current);
    }
    return total_path;
}

void get_neighbors(Vec2i at, std::vector<Vec2i> &neighbors) {
    std::vector<Vec2i> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (const Vec2i &dir : directions) {
        Vec2i check_pos = {at.x + dir.x, at.y + dir.y};

        if (check_pos.x >= 0 && check_pos.x < 16 && check_pos.y >= 0 &&
            check_pos.y < 16) {
            neighbors.push_back(check_pos);
        }
    }
}

float get_cell_weight(Vec2i at) {
    if (graph[at.x][at.y] == 0) {
        return 1.0;
    } else {
        return std::numeric_limits<float>::infinity();
    }
}

float h(Vec2i from, Vec2i to) {
    float dx = powf(to.x - from.x, 2.0);
    float dy = powf(to.y - from.y, 2.0);
    return sqrtf(dx + dy);
}

std::vector<Vec2i> get_path(Vec2i start_position, Vec2i target_position) {
    std::priority_queue<Pair> frontier;
    frontier.push(Pair{start_position, 0.0});

    std::unordered_map<Vec2i, Vec2i> came_from;

    std::unordered_map<Vec2i, float> g_score;
    g_score[start_position] = 0.0;

    auto neighbors = std::vector<Vec2i>();

    while (!frontier.empty()) {
        Pair current = frontier.top();
        frontier.pop();
        if (current.position == target_position) {
            return construct_path(came_from, current.position);
        }

        get_neighbors(current.position, neighbors);
        for (const Vec2i &next : neighbors) {
            float tentative_g_score =
                g_score[current.position] +
                h(current.position, next) * get_cell_weight(next);

            if (g_score.find(next) == g_score.end() ||
                tentative_g_score < g_score[next]) {
                frontier.push(
                    Pair{next, tentative_g_score + h(next, target_position)});
                g_score[next] = tentative_g_score;
                came_from[next] = current.position;
            }
        }
        neighbors.clear();
    }

    return std::vector<Vec2i>();
}

int main() {
    int start_pos_x = 0;
    std::cout << "Start X: ";
    std::cin >> start_pos_x;
    int start_pos_y = 0;
    std::cout << "Start Y: ";
    std::cin >> start_pos_y;
    int end_pos_x = 0;

    std::cout << "End X: ";
    std::cin >> end_pos_x;
    int end_pos_y = 0;
    std::cout << "End Y: ";
    std::cin >> end_pos_y;

    Vec2i start_pos = {start_pos_x, start_pos_y};
    Vec2i end_pos = {end_pos_x, end_pos_y};
    if (start_pos.x < 0 || start_pos.x > 15 || start_pos.y < 0 ||
        start_pos.y > 15) {
        std::cerr << "Start position is out of range" << std::endl;
        return 1;
    }
    if (end_pos.x < 0 || end_pos.x > 15 || end_pos.y < 0 || end_pos.y > 15) {
        std::cerr << "End position is out of range" << std::endl;
        return 1;
    }

    auto time = std::chrono::steady_clock::now();

    auto path = get_path(start_pos, end_pos);

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                       std::chrono::steady_clock::now() - time)
                       .count();

    for (const Vec2i &pos : path) {
        graph[pos.x][pos.y] = 8;
    }

    std::cout << "0: empty cell, 1: occupied cell, 8: path" << std::endl;
    for (const auto &row : graph) {
        for (const auto &element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Path found in " << elapsed << " µs" << std::endl;
    ;

    return 0;
}
