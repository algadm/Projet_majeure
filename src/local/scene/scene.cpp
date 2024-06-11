

#include <GL/glew.h>

#include "scene.hpp"
#include "../../lib/3d/mat2.hpp"

#include "../../lib/opengl/glutils.hpp"

#include "../../lib/perlin/perlin.hpp"
#include "../../lib/interface/camera_matrices.hpp"

#include "../interface/myWidgetGL.hpp"
#include "../path/path.hpp"

#include <cmath>

#include <string>
#include <sstream>
#include <fstream>
#include "../../lib/mesh/mesh_io.hpp"


using namespace cpe;


void scene::build_surface(int const Nu, int const Nv)
{
    surface.set_plane_xy_unit(Nu,Nv);

    // Read binary file
    std::ifstream file("data/heightmap.bin", std::ios::binary);
    if (!file)
        std::cerr << "Error opening file!" << std::endl;

    std::vector<uint16_t> heightmap(Nu * Nv);
    file.read(reinterpret_cast<char*>(heightmap.data()), heightmap.size() * sizeof(uint16_t));

    if (!file)
        std::cerr << "Error reading file!" << std::endl;

    file.close();

    // Give height to plane map
    for (int ku = 0; ku < Nu; ++ku)
    {
        for (int kv = 0; kv < Nv; ++kv)
        {
            int index = kv * Nu + ku;
            int height = heightmap[index];

            floor_color(ku,kv,height);

            float const x = kv;
            float const y = ku;
            float const z = .1f*height;
            surface.vertex(ku, kv) = {x, y, z};
        }
    }
    surface.transform_apply_auto_scale_and_center();
}

void scene::display(ivec2 const start, ivec2 const goal)
{
    town_color(start.x(), start.y(), 1.0f, 0.0f, 1.0f);
    town_color(goal.x(), goal.y(), 0.0f, 1.0f, 1.0f);
}

void scene::town_color(int const x, int const y, float const R, float const G, float const B)
{
    surface.color(x,y) = {R,G,B};
    surface.color(x+1,y) = {R,G,B};
    surface.color(x+1,y-1) = {R,G,B};
    surface.color(x+1,y+1) = {R,G,B};
    surface.color(x-1,y) = {R,G,B};
    surface.color(x-1,y-1) = {R,G,B};
    surface.color(x-1,y+1) = {R,G,B};
    surface.color(x,y+1) = {R,G,B};
    surface.color(x,y-1) = {R,G,B};
}

void scene::road_color(int const x, int const y)
{
    surface.color(x,y) = {0.0f, 0.0f, 0.0f};
}

void scene::floor_color(int const ku, int const kv, int const height)
{
    if (height <= 200)
        surface.color(ku,kv) = {0.0f, 0.6f, 0.0f};

    if (height <= 240 && height > 200)
        surface.color(ku,kv) = {0.0f, 0.7f, 0.0f};

    if (height <= 290 && height > 240)
        surface.color(ku,kv) = {0.8f, 0.8f, 0.0f};

    if (height <= 320 && height > 290)
        surface.color(ku,kv) = {0.8f, 0.7f, 0.0f};

    if (height <= 360 && height > 320)
        surface.color(ku,kv) = {0.8f, 0.5f, 0.0f};

    if (height <= 430 && height > 360)
        surface.color(ku,kv) = {0.8f, 0.0f, 0.0f};

    if (height > 430)
        surface.color(ku,kv) = {0.8f, 0.8f, 0.8f};
}

void scene::load_scene()
{
    load_common_data();

    // World size
    int const Nu = 571;
    int const Nv = 346;

    ivec2 start = {65,90};
    ivec2 goal = {40,90};

    build_surface(Nu,Nv);
    display(start, goal);

    path path(Nv, Nu, start, goal);
    path.A_star(surface);

    for (int i = 0; i < path.data.size(); i++)
        road_color(path.data[i].x(), path.data[i].y());


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

