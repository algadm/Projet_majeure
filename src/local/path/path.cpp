#include <GL/glew.h>

#include "path.hpp"

#include "../interface/myWidgetGL.hpp"
#include "../../lib/mesh/mesh_io.hpp"

#include <cmath>
#include <string>
#include <sstream>
#include <limits>
#include <array>
#include <queue>
#include <utility>
#include <iostream>
#include <vector>

using namespace cpe;

/** Min-heap comparator */
struct CompareVec
{
    bool operator()(const std::pair<float, cpe::ivec2>& pair1, const std::pair<float, cpe::ivec2>& pair2) const
    {
        return pair1.first > pair2.first;
    }
};

path::path(int const n, int const m, cpe::ivec2 Start, cpe::ivec2 Goal) : N(n), M(m), start(Start), goal(Goal)
{
    heuristics.resize(N * M);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) 
            heuristics[i * M + j] = h(cpe::ivec2(i, j));

    mask = {cpe::ivec2(0, -1), cpe::ivec2(0, 1), cpe::ivec2(-1, 0), cpe::ivec2(1, 0),
            cpe::ivec2(-1, -1), cpe::ivec2(-1, 1), cpe::ivec2(1, -1), cpe::ivec2(1, 1),
            cpe::ivec2(-1, -2), cpe::ivec2(-1, 2), cpe::ivec2(1, -2), cpe::ivec2(1, 2),
            cpe::ivec2(-2, -1), cpe::ivec2(-2, 1), cpe::ivec2(2, -1), cpe::ivec2(2, 1)};

    distance = {1.0f, 1.0f, 1.0f, 1.0f,
                std::sqrt(2.0f), std::sqrt(2.0f), std::sqrt(2.0f), std::sqrt(2.0f),
                std::sqrt(5.0f), std::sqrt(5.0f), std::sqrt(5.0f), std::sqrt(5.0f),
                std::sqrt(5.0f), std::sqrt(5.0f), std::sqrt(5.0f), std::sqrt(5.0f)};

    g_score.resize(N * M, std::numeric_limits<float>::infinity());
    f_score.resize(N * M, std::numeric_limits<float>::infinity());
}

void path::reconstruct_path(std::vector<cpe::ivec2>& came_from, cpe::ivec2 current)
{
    while (current != start)
    {
        data.push_back(current);
        current = came_from[current.x() * M + current.y()];
    }
    data.push_back(start);
    std::reverse(data.begin(), data.end());
}

float path::h(cpe::ivec2 ij) const
{
    return std::sqrt(static_cast<float>((ij.x() - goal.x()) * (ij.x() - goal.x()) + (ij.y() - goal.y()) * (ij.y() - goal.y())));
}

float path::g(cpe::ivec2 ij1, cpe::ivec2 ij2, int n, cpe::mesh_parametric surface) const
{
    float k = std::abs(surface.vertex(ij1.x(), ij1.y()).z() - surface.vertex(ij2.x(), ij2.y()).z()) / distance[n];
    k *= 5000.0f;
    return k;
}

int path::A_star(cpe::mesh_parametric surface)
{
    std::priority_queue<std::pair<float, cpe::ivec2>, std::vector<std::pair<float, cpe::ivec2>>, CompareVec> priority_q;
    std::vector<cpe::ivec2> came_from(N * M, cpe::ivec2(-1, -1));

    g_score[start.x() * M + start.y()] = 0;
    f_score[start.x() * M + start.y()] = h(start);

    priority_q.push({h(start), start});

    while (!priority_q.empty())
    {
        auto current_pair = priority_q.top();
        cpe::ivec2 current = current_pair.second;
        priority_q.pop();

        std::cout << "Current node: (" << current.x() << ", " << current.y() << ")\n";

        if (current == goal)
        {
            reconstruct_path(came_from, current);
            return 1;
        }

        for (int n = 0; n < mask.size(); n++)
        {
            cpe::ivec2 neighbor = current + mask[n];
            if (neighbor.x() >= 0 && neighbor.x() < N && neighbor.y() >= 0 && neighbor.y() < M)
            {
                float tentative_g_score = g_score[current.x() * M + current.y()] + g(current, neighbor, n, surface);

                if (tentative_g_score < g_score[neighbor.x() * M + neighbor.y()])
                {
                    came_from[neighbor.x() * M + neighbor.y()] = current;
                    g_score[neighbor.x() * M + neighbor.y()] = tentative_g_score;
                    float f_score_value = tentative_g_score + heuristics[neighbor.x() * M + neighbor.y()];
                    f_score[neighbor.x() * M + neighbor.y()] = f_score_value;
                    priority_q.push({f_score_value, neighbor});
                } 
            }
        }
    }
    return 0;
}
