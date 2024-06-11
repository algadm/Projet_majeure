#include <GL/glew.h>

#include "path.hpp"

#include "../interface/myWidgetGL.hpp"
#include "../../lib/mesh/mesh_io.hpp"

#include <cmath>
#include <string>
#include <sstream>
#include <limits>
#include <array>



using namespace cpe;

path::path() {}

void path::reconstruct_path(std::vector<cpe::ivec2> came_from, cpe::ivec2 current, int N)
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

const static std::array<cpe::ivec2,16> mask = {cpe::ivec2(0,-1),cpe::ivec2(0,1),cpe::ivec2(-1,0),cpe::ivec2(1,0),
                                                cpe::ivec2(-1,-1),cpe::ivec2(-1,1),cpe::ivec2(1,-1),cpe::ivec2(1,1),
                                                cpe::ivec2(-1,-2),cpe::ivec2(-1,2),cpe::ivec2(1,-2),cpe::ivec2(1,2),
                                                cpe::ivec2(-2,-1),cpe::ivec2(-2,1),cpe::ivec2(2,-1),cpe::ivec2(2,1)};

const static std::array<float,16> distance = {1.0f,1.0f,1.0f,1.0f,
                                            std::sqrt(2.0f),std::sqrt(2.0f),std::sqrt(2.0f),std::sqrt(2.0f),
                                            std::sqrt(5.0f),std::sqrt(5.0f),std::sqrt(5.0f),std::sqrt(5.0f),
                                            std::sqrt(5.0f),std::sqrt(5.0f),std::sqrt(5.0f),std::sqrt(5.0f),};

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

bool path::lower_score(cpe::ivec2 v1, cpe::ivec2 v2, std::array<float,int NM> f_score, int N)
{
    return (f_score[v1[0]*N+v1[1]] < f_score[v2[0]*N+v2[1]]);
}

struct lower_score {
    int N;
    std::array<cpe::ivec2, int NM> score;
    lower_score()
}


std::vector<cpe::ivec2> path::A_star(cpe::ivec2 start,cpe::ivec2 goal,int N,int M)
{
    std::priority_queue<cpe::ivec2,lower()> priority_q;
    priority_q.push(start);
    std::array<bool,N*M> in_p_q; // default value = false
    in_p_q[start[0]*N+start[1]] = true;

    std::array<cpe::ivec2,N*M> came_from;

    std::array<float,N*M> g_score;
    for (float& score : g_score){
        score = std::numeric_limits<float>::infinity();
    }
    g_score[start[0]*N+start[1]] = 0;

    std::array<float,N*M> f_score;
    for (float& score : f_score){
        score = std::numeric_limits<float>::infinity();
    }
    f_score[start[0]*N+start[1]] = h(start);

    while (priority_q.size() > 0) {
        cpe::ivec2 current = priority_q.top();
        if (current == goal) {
            return reconstruct_path(came_from, current);
        }
        priority_q.pop();
        in_p_q[current[0]*N+current[1]] = false;

        for (int n = 0; n < 16; n++) {
            ivec2 neighbor = current + mask[n];
            if (neighbor[0]>=0 && neighbor[0]<N && neighbor[1]>=0 && neighbor[1]<M) {
                float tentative_score = g_score[current[0]*N+current[1]] + distance[i];
                if (tentative_score < g_score[neighbor[0]*N+neighbor[1]]) {
                    came_from[neighbor[0]*N+neighbor[1]] = current;
                    g_score[neighbor[0]*N+neighbor[1]] = tentative_score;
                    f_score[neighbor[0]*N+neighbor[1]] = tentative_score + h(neighbor);
                    if (in_p_q[neighbor[0]*N+neighbor[1]] == false){
                        priority_q.push(neighbor);
                        in_p_q[neighbor[0]*N+neighbor[1]] = true;
                    }
                }
            }
            
        }
    }


}

