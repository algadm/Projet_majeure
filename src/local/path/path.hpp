
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



class myWidgetGL;

class path
{
public:
    path(int const N, int const M);
    void reconstruct_path(std::vector<cpe::ivec2> came_from, cpe::ivec2 current);
    void add_vertex_to_path(cpe::ivec2 ij);
    void pop_last_vertex_from_path();
    void set_start(cpe::ivec2 b);
    void set_goal(cpe::ivec2 e);
    float h(cpe::ivec2 ij);
    float g(cpe::ivec2 ij1, cpe::ivec2 ij2, int n, cpe::mesh_parametric surface);

    int A_star(cpe::ivec2 start, cpe::ivec2 goal, cpe::mesh_parametric surface);




private:
    // data is a std::vector with ivec2 (i,j) inside
    std::vector<cpe::ivec2> data;
    std::array<cpe::ivec2,16> mask;
    std::array<float,16> distance;
    cpe::ivec2 start;
    cpe::ivec2 goal;
    int N;
    int M;
    std::vector<float> heuristics;
    cpe::mesh_parametric surface;
    
    

};

#endif
