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



using namespace cpe;

path::path(int const n, int const m) {
    N = n;
    M = m;
    for (int i = 0; i < N ; i++) {
        for (int j = 0; j < M ; j++) {
            heuristics.push_back(h(cpe::ivec2(i,j)));
        }
    }
    mask = {cpe::ivec2(0,-1),cpe::ivec2(0,1),cpe::ivec2(-1,0),cpe::ivec2(1,0),
            cpe::ivec2(-1,-1),cpe::ivec2(-1,1),cpe::ivec2(1,-1),cpe::ivec2(1,1),
            cpe::ivec2(-1,-2),cpe::ivec2(-1,2),cpe::ivec2(1,-2),cpe::ivec2(1,2),
            cpe::ivec2(-2,-1),cpe::ivec2(-2,1),cpe::ivec2(2,-1),cpe::ivec2(2,1)};

    distance = {1.0f,1.0f,1.0f,1.0f,
        std::sqrt(2.0f),std::sqrt(2.0f),std::sqrt(2.0f),std::sqrt(2.0f),
        std::sqrt(5.0f),std::sqrt(5.0f),std::sqrt(5.0f),std::sqrt(5.0f),
        std::sqrt(5.0f),std::sqrt(5.0f),std::sqrt(5.0f),std::sqrt(5.0f),};
}

void path::reconstruct_path(std::vector<cpe::ivec2> came_from, cpe::ivec2 current)
{
    data.push_back(current);
    while (current[0] != start[0] && current[1] != start[1]) {
        current = came_from[current[0]*N+current[1]];
        data.push_back(current);
    }
}

void path::add_vertex_to_path(ivec2 ij)
{
    data.push_back(ij);
}

void path::pop_last_vertex_from_path()
{
    data.pop_back();
}

void path::set_start(cpe::ivec2 b)
{
    start = b;
}

void path::set_goal(cpe::ivec2 e)
{
    goal = e;
}


// Heuristic function
float path::h(ivec2 ij)
{
    return std::sqrt(static_cast<float>((ij[0]-goal[0])*(ij[0]-goal[0]) + (ij[1]-goal[1])*(ij[1]-goal[1])));
}



// from ij1 to ij2
float path::g(ivec2 ij1, ivec2 ij2)
{
    return std::sqrt(static_cast<float>((ij1[0]-ij2[0])*(ij1[0]-ij2[0]) + (ij1[1]-ij2[1])*(ij1[1]-ij2[1])));
}

// f = g + h
float path::f(ivec2 ij1, ivec2 ij2)
{
    return g(ij1,ij2) + h(ij2);
}


int path::A_star(cpe::ivec2 start,cpe::ivec2 goal)
{
    std::priority_queue<std::pair<float,cpe::ivec2>> priority_q;
    float f = heuristics[start[0]*N+start[1]];
    std::pair<float,cpe::ivec2> p;
    p.first = f;
    p.second = start;
    priority_q.push(p);

    std::vector<cpe::ivec2> came_from;
    std::vector<float> g_score;
    std::vector<float> f_score;
    for (int i = 0; i < N*M ; i++) {
        came_from.push_back(ivec2(-1,-1));
        g_score.push_back(std::numeric_limits<float>::infinity());
        f_score.push_back(std::numeric_limits<float>::infinity());
    }

    g_score[start[0]*N+start[1]] = 0;
    f_score[start[0]*N+start[1]] = h(start);

    while (priority_q.size() > 0) {
        std::pair<float,cpe::ivec2> current_pair = priority_q.top();
        cpe::ivec2 current = current_pair.second;
        if (current == goal) {
            reconstruct_path(came_from, current);
            return 1;
        }
        priority_q.pop();

        for (int n = 0; n < 16; n++) {
            cpe::ivec2 neighbor = current + mask[n];
            if (neighbor[0]>=0 && neighbor[0]<N && neighbor[1]>=0 && neighbor[1]<M) {
                float tentative_score = g_score[current[0]*N+current[1]] + distance[n];
                if (tentative_score < g_score[neighbor[0]*N+neighbor[1]]) {
                    came_from[neighbor[0]*N+neighbor[1]] = current;
                    g_score[neighbor[0]*N+neighbor[1]] = tentative_score;
                    f_score[neighbor[0]*N+neighbor[1]] = tentative_score + h(neighbor);
                    f = f_score[neighbor[0]*N+neighbor[1]];
                    p.first = f;
                    p.second = neighbor;
                    priority_q.push(p);
                    }
                }
            }
    }
    return 0;
}


