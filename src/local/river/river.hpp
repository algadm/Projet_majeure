#pragma once

#ifndef RIVER_HPP
#define RIVER_HPP

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

class river
{
public:
    river(int const n, int const m, float t, cpe::mesh_parametric s);
    void connected_components(int min_size);

    void update_water(int min_size);
    std::vector<float> get_water();



private:
    /** World size */
    int N;
    int M;
    float thresh;
    std::vector<int> labels;
    std::vector<int> data;
    std::vector<float> water;
};

#endif