

#include <GL/glew.h>

#include "scene.hpp"

#include "../../lib/opengl/glutils.hpp"
#include "../../lib/perlin/perlin.hpp"
#include "../../lib/interface/camera_matrices.hpp"
#include "../interface/myWidgetGL.hpp"
#include "../../lib/mesh/mesh_io.hpp"

#include <cmath>
#include <string>
#include <sstream>


using namespace cpe;


void scene::load_scene()
{

    //*****************************************//
    // Preload default structure               //
    //*****************************************//
    texture_default = load_texture_file("data/white.jpg");
    shader_program_id = read_shader("shaders/shader_mesh.vert",
                                    "shaders/shader_mesh.frag");


    //*****************************************//
    // OBJ Mesh                                //
    //*****************************************//
    mesh_ground = load_mesh_file("data/gebco_08_rev_elev_A1_grey_geo.obj");
    mesh_ground.transform_apply_auto_scale_and_center();
    // mesh_ground.transform_apply_rotation({1.0f,0.0f,0.0f},-M_PI/2.0f);
    mesh_ground.transform_apply_rotation({0.0f,0.0f,1.0f}, M_PI/2.0f);
    mesh_ground.fill_color_xyz();
    mesh_ground_opengl.fill_vbo(mesh_ground);
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


    //Draw the meshes
    mesh_ground_opengl.draw();

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

void scene::flat_surface(float xmin,float xmax, float zmin, float zmax, int Nu, int Nv)
{
    const float dx = (xmax-xmin)/static_cast<float>(Nu-1);
    const float dz = (zmax-zmin)/static_cast<float>(Nv-1);

    for (int kv = 0; kv < Nv; kv++) {
        for (int ku = 0; ku < Nu; ku++) {
            mesh_ground.add_vertex( {xmin + ku*dx,-0.25f,zmin + kv*dz} );
        }
    }
    for (int kv = 0; kv < Nv - 1; kv++) {
        for (int ku = 0; ku < Nu - 1; ku++) {
            mesh_ground.add_triangle_index({ku+kv*Nu,ku+1+kv*Nu,ku+(kv+1)*Nu});
            mesh_ground.add_triangle_index({ku+1+kv*Nu,ku+1+(kv+1)*Nu,ku+(kv+1)*Nu});
        }
    }
}

void scene::trigo_surface(float xmin,float xmax, float zmin, float zmax, int Nu, int Nv)
{
    const float dx = (xmax-xmin)/static_cast<float>(Nu-1);
    const float dz = (zmax-zmin)/static_cast<float>(Nv-1);

    for (int kv = 0; kv < Nv; kv++) {
        for (int ku = 0; ku < Nu; ku++) {
            mesh_ground.add_vertex( {xmin + ku*dx,sin(ku*dx)+cos(kv*dz)-1.25f,zmin + kv*dz} );
        }
    }
    for (int kv = 0; kv < Nv - 1; kv++) {
        for (int ku = 0; ku < Nu - 1; ku++) {
            mesh_ground.add_triangle_index({ku+kv*Nu,ku+1+kv*Nu,ku+(kv+1)*Nu});
            mesh_ground.add_triangle_index({ku+1+kv*Nu,ku+1+(kv+1)*Nu,ku+(kv+1)*Nu});
        }
    }
}

void scene::perlin_surface(float xmin,float xmax, float zmin, float zmax, int Nu, int Nv)
{
    const float dx = (xmax-xmin)/static_cast<float>(Nu-1);
    const float dz = (zmax-zmin)/static_cast<float>(Nv-1);
    perlin p = perlin(8,0.5);

    for (int kv = 0; kv < Nv; kv++) {
        for (int ku = 0; ku < Nu; ku++) {
            vec2 point = {xmin + ku*dx,zmin + kv*dz};
            mesh_ground.add_vertex( {point.x(),0.1f*p(point),point.y()} );
        }
    }
    for (int kv = 0; kv < Nv - 1; kv++) {
        for (int ku = 0; ku < Nu - 1; ku++) {
            mesh_ground.add_triangle_index({ku+kv*Nu,ku+1+kv*Nu,ku+(kv+1)*Nu});
            mesh_ground.add_triangle_index({ku+1+kv*Nu,ku+1+(kv+1)*Nu,ku+(kv+1)*Nu});
        }
    }
}
