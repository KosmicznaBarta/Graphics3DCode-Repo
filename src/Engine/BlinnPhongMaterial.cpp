#include "Material.h"

#include "BlinnPhongMaterial.h"

#include "AbstractMaterial.h"

#include "Application/utils.h"

#include "Engine/utils.h"

#include "Engine/mesh_loader.h"

#include "texture.h"

#include "ObjectReader/sMesh.h"

GLint xe::BlinnPhongMaterial::map_Kd_location_ = -1;

namespace xe {
    BlinnPhongMaterial::BlinnPhongMaterial(const glm::vec4 &Ka, const glm::vec4 &Kd, int use_colors) : Ka_(Ka), Kd_(Kd), use_vertex_colors_(use_colors), texture_(0) {}
    BlinnPhongMaterial::BlinnPhongMaterial(const glm::vec4 &Ka, const glm::vec4 &Kd, int use_colors, int texture) : Ka_(Ka), Kd_(Kd), use_vertex_colors_(use_colors), texture_(texture) {}
    BlinnPhongMaterial::BlinnPhongMaterial(const glm::vec4 &Ka, const glm::vec4 &Kd, const glm::vec3 &Ks, float Ns, int use_colors) : Ka_(Ka), Kd_(Kd), Ks_(Ks), Ns_(Ns), use_vertex_colors_(use_colors), texture_(0) {}
    BlinnPhongMaterial::BlinnPhongMaterial(const glm::vec4 &Ka, const glm::vec4 &Kd, const glm::vec3 &Ks, float Ns, int use_colors, int texture) : Ka_(Ka), Kd_(Kd),Ks_(Ks), Ns_(Ns), use_vertex_colors_(use_colors), texture_(texture) {}

    void BlinnPhongMaterial::init() {
        create_material_uniform_buffer(4 * sizeof(glm::vec4));

        create_program_in_engine({{GL_VERTEX_SHADER, "BlinnPhong_vs.glsl"},
                              {GL_FRAGMENT_SHADER, "BlinnPhong_fs.glsl"}});

        map_Kd_location_ = glGetUniformLocation(program(), "map_Kd");
        if (map_Kd_location_ == -1) {
            SPDLOG_WARN("Cannot find map_Kd uniform");
        }

        xe::add_mat_function("BlinnPhongMaterial", BlinnPhongMaterial::create_from_mtl);
    }

    void BlinnPhongMaterial::bind() const {
        glUseProgram(program());
        
        int use_map_Kd_ = 0;
        
        if (texture_ > 0) {
            use_map_Kd_ = 1;
            OGL_CALL(glActiveTexture(GL_TEXTURE0));
            OGL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
            OGL_CALL(glUniform1i(map_Kd_location_, 0));
        }
        glm::vec4 KsNs_(Ks_, Ns_);
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, material_uniform_buffer()));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Ka_)); 
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &Kd_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(glm::vec4), sizeof(glm::vec4), &KsNs_)); 
        //OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(glm::vec4) + sizeof(glm::vec3), sizeof(float), &Ns_)); 
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 3*sizeof(glm::vec4), sizeof(int), &use_vertex_colors_)); 
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 3*sizeof(glm::vec4) + sizeof(int), sizeof(int), &use_map_Kd_)); 
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , material_uniform_buffer()));
    }

    void BlinnPhongMaterial::unbind() const {
        if (texture_ > 0) {
            OGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        }

        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0));
    }

    void BlinnPhongMaterial::set_texture(GLint texture) {texture_ = texture;}

    Material *BlinnPhongMaterial::create_from_mtl(const mtl_material_t &mat, std::string mtl_dir) {
        glm::vec4 Ka_(mat.ambient[0], mat.ambient[1], mat.ambient[2], 1.0f);
        glm::vec3 Ks_(mat.specular[0], mat.specular[1], mat.specular[2]); 
        float Ns_(mat.shininess);
        glm::vec4 color = get_color(mat.diffuse);
        SPDLOG_DEBUG("Adding ColorMaterial {}", glm::to_string(color));
        //auto material = new xe::BlinnPhongMaterial(Ka_, color, 0);
        auto material = new xe::BlinnPhongMaterial(Ka_, color,Ks_, Ns_, 0);
        if (!mat.diffuse_texname.empty()) {
            auto texture = xe::create_texture(mtl_dir + "/" + mat.diffuse_texname, true);
            SPDLOG_DEBUG("Adding Texture {} {:1d}", mat.diffuse_texname, texture);
            if (texture > 0) {
                material->set_texture(texture);
            }
        }

        return material;
   }
}