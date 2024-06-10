

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
#include <fstream>
#include "../../lib/mesh/mesh_io.hpp"


using namespace cpe;


void scene::build_surface()
{
    int const Nu = 571;
    int const Nv = 346;

    surface.set_plane_xy_unit(Nu,Nv);

    std::ifstream file("data/heightmap.bin", std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        // continue;
    }

    // Read the data into a vector
    std::vector<uint16_t> heightmap(Nu * Nv);
    file.read(reinterpret_cast<char*>(heightmap.data()), heightmap.size() * sizeof(uint16_t));

    if (!file) {
        std::cerr << "Error reading file!" << std::endl;
        // continue;
    }

    file.close();

    // Process the heightmap data
    // For example, print the first few values
    for (int ku = 0; ku < Nu; ++ku) {
        for (int kv = 0; kv < Nv; ++kv) {
            // Calculate the index in the heightmap array
            int index = kv * Nu + ku;

            // Get the height value
            uint16_t height = heightmap[index];

            float const x = ku;
            float const y = kv;
            float const z = .1f*height;
            surface.vertex(ku, kv) = {x, y, z};
        }
    }
    surface.transform_apply_auto_scale_and_center();
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

