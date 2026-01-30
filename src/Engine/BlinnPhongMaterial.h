#pragma once

#include "Engine/AbstractMaterial.h"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "spdlog/spdlog.h"
#include "ObjectReader/sMesh.h"

namespace xe {
    class BlinnPhongMaterial : public AbstractMaterial<BlinnPhongMaterial> {
        public:
            BlinnPhongMaterial(const glm::vec4 &Ka, const glm::vec4 &Kd, int use_colors);
            BlinnPhongMaterial(const glm::vec4 &Ka, const glm::vec4 &Kd, int use_colors, int texture);

            static void init();

            void bind() const override;

            void unbind() const override;

            static Material *create_from_mtl(const mtl_material_t &mat, std::string mtl_dir);

            void set_texture(GLint texture);

        private:
            glm::vec4 Kd_;
            glm::vec4 Ka_;
            int use_vertex_colors_;
            static GLint map_Kd_location_;
            GLint texture_;
    };
}