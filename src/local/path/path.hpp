
#pragma once

#ifndef PATH_HPP
#define PATH_HPP

//#include "GL/gl.h"
#include <GL/glew.h>

#include "../../lib/3d/mat3.hpp"
#include "../../lib/3d/vec3.hpp"
#include "../../lib/3d/ivec2.hpp"
#include "../../lib/mesh/mesh.hpp"
#include "../../lib/mesh/mesh_parametric.hpp"
#include "../../lib/opengl/mesh_opengl.hpp"
#include "../../lib/interface/camera_matrices.hpp"

#include <vector>
#include <cmath>
#include <array>
#include <unordered_map>



class myWidgetGL;

class path
{
public:
    path(int const N, int const M, cpe::ivec2 start, cpe::ivec2 goal);

    /** Fills data with the shortest path */
    void reconstruct_path(std::vector<cpe::ivec2>& came_from, cpe::ivec2 current);

    /** Heuristic function */ 
    float h(cpe::ivec2 ij) const;
    /** Cost function */
    float g(cpe::ivec2 ij1, cpe::ivec2 ij2, int n, cpe::mesh_parametric surface) const;

    /** Performs the A* algorithm */
    int A_star(cpe::mesh_parametric surface);

    /** data is a std::vector with ivec2 (i,j) inside */
    std::vector<cpe::ivec2> data;

    int Dijkstra(cpe::mesh_parametric surface);


private:
    /** World size */
    int N;
    int M;

    /** Coordinates of the start and goal points */
    cpe::ivec2 start;
    cpe::ivec2 goal;

    std::vector<float> g_score;
    std::vector<float> f_score;

    cpe::mesh_parametric surface;

    std::array<float,16> distance;
    std::array<cpe::ivec2,16> mask;

    std::vector<float> heuristics;
};

#endif
