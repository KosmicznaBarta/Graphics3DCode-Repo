#include "Material.h"

#include "KdMaterial.h"

#include "AbstractMaterial.h"

#include "Application/utils.h"

namespace xe {
    KdMaterial::KdMaterial(const glm::vec4 &Kd ) : Kd_(Kd), use_vertex_colors_(false) {}
    KdMaterial::KdMaterial(const glm::vec4 &Kd, int use_colors) : Kd_(Kd), use_vertex_colors_(use_colors) {}

    void KdMaterial::init() {
        create_material_uniform_buffer(2*sizeof(glm::vec4));

        create_program_in_engine({{GL_VERTEX_SHADER, "Kd_vs.glsl"},
                              {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});
    }

    void KdMaterial::bind() const {
        glUseProgram(program());
        
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , material_uniform_buffer()));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(int), &use_vertex_colors_));
    }

    void KdMaterial::unbind() const {
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0));
    }
}