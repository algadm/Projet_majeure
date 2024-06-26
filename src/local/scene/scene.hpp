
/** TP 4ETI - CPE Lyon - 2013/2014 */

#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

//#include <GL/gl.h>
#include <GL/glew.h>

#include "../path/path.hpp"
#include "../../lib/3d/mat3.hpp"
#include "../../lib/3d/vec3.hpp"
#include "../../lib/3d/ivec2.hpp"
#include "../../lib/mesh/mesh_parametric.hpp"
#include "../../lib/opengl/mesh_opengl.hpp"
#include "../../lib/interface/camera_matrices.hpp"

#include <vector>


class myWidgetGL;

class scene
{
public:

    scene();

    
    void build_surface(int const Nu, int const Nv, path& path);


    /** \brief Method called only once at the beginning (load off files ...) */
    void load_scene();

    /** \brief Method called at every frame */
    void draw_scene();

    /** Set the pointer to the parent Widget */
    void set_widget(myWidgetGL* widget_param);

    /** Load basic data for the scene */
    void load_common_data();

    /** Displays the elements of the A* algorithm */
    void display(cpe::ivec2 const start, cpe::ivec2 const goal);

    /** Displays the color of the town */
    void town_color(int const x, int const y, float const R, float const G, float const B);

    /** Displays the road */
    void road_color(int const x, int const y);

    /** Displays the ground */
    void floor_color(int const ku, int const kv, int const height);


private:

    /** Load a texture from a given file and returns its id */
    GLuint load_texture_file(std::string const& filename);

    /** Access to the parent object */
    myWidgetGL* pwidget;

    /** Default id for the texture (white texture) */
    GLuint texture_default;

    /** The id of the shader do draw meshes */
    GLuint shader_program_id;


    cpe::mesh_parametric surface;
    cpe::mesh_opengl surface_opengl;


};

#endif
