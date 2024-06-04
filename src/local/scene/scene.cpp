

#include <GL/glew.h>

#include "scene.hpp"
#include "../../lib/3d/mat2.hpp"

#include "../../lib/opengl/glutils.hpp"

#include "../../lib/perlin/perlin.hpp"
#include "../../lib/interface/camera_matrices.hpp"

#include "../interface/myWidgetGL.hpp"

#include <cmath>

#include <string>
#include <sstream>
#include "../../lib/mesh/mesh_io.hpp"


using namespace cpe;


void scene::build_surface()
{
    int Nu = 200;
    int Nv = 200;

    surface.set_plane_xy_unit(Nu,Nv);

    // float const u_min = -2.0f;
    // float const u_max = 2.0f;

    // // Precompute step sizes
    // float const du = (u_max - u_min) / (Nu - 1);
    // float const dv = 2*M_PI / (Nv - 1);
    
    // for(int ku=0 ; ku<Nu ; ++ku)
    // {
    //     for(int kv=0 ; kv<Nv ; ++kv)
    //     {
    //         float const u = u_min + static_cast<float>(ku)/(Nu-1) * (u_max-u_min);
    //         float const v = 2*M_PI * static_cast<float>(kv)/(Nv-1) - M_PI;

    //         float const x = cos(v)/cosh(u);
    //         float const y = sin(v)/cosh(u);
    //         float const z = (u-tanh(u));

    //         surface.vertex(ku,kv) = {x,y,z};
    //         surface.color(ku,kv) = {x,y,z};
    //     }
    // }
}

void scene::load_scene()
{
    load_common_data();


    build_surface();


    surface.fill_normal();
    surface.fill_empty_field_by_default();

    surface_opengl.fill_vbo(surface);

}

void scene::draw_scene()
{
    //Setup uniform parameters
    glUseProgram(shader_program_id);                                                                           PRINT_OPENGL_ERROR();

    //Get cameras parameters (modelview,projection,normal).
    camera_matrices const& cam=pwidget->camera();

    //Set Uniform data to GPU
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"camera_modelview"),1,false,cam.modelview.pointer());     PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"camera_projection"),1,false,cam.projection.pointer());   PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"normal_matrix"),1,false,cam.normal.pointer());           PRINT_OPENGL_ERROR();


    glBindTexture(GL_TEXTURE_2D,texture_default);  PRINT_OPENGL_ERROR();
    surface_opengl.draw();
}


scene::scene()
    :shader_program_id(0)
{}


GLuint scene::load_texture_file(std::string const& filename)
{
    return pwidget->load_texture_file(filename);
}

void scene::set_widget(myWidgetGL* widget_param)
{
    pwidget=widget_param;
}


void scene::load_common_data()
{
    texture_default=load_texture_file("data/white.jpg");
    shader_program_id = read_shader("shaders/shader_mesh.vert",
                                    "shaders/shader_mesh.frag"); PRINT_OPENGL_ERROR();
}

