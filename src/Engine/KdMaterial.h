#pragma once

#include "Engine/AbstractMaterial.h"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "spdlog/spdlog.h"

namespace xe {
    class KdMaterial : public AbstractMaterial<KdMaterial> {
        public:
            KdMaterial(const glm::vec4 &Kd , int use_colors);

            static void init();

            void bind() const override;

            void unbind() const override;

        private:
            glm::vec4 Kd_;
            int use_vertex_colors_;
    };
}