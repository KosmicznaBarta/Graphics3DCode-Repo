//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "glad/gl.h"

#include "Application/application.h"

#include "glm/glm.hpp"


class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug, int swap_interval = 1) : Application(
            width, height, title,
            debug, swap_interval) {}

    void framebuffer_resize_callback(int w, int h) override;

    float fov_;
    float aspect_;
    float near_;
    float far_;
    
    glm::mat4 P_;
    glm::mat4 V_;
    glm::mat4 M_;

    GLuint u_trans_buffer_handle_;

    void init() override;

    void frame() override;

private:
    GLuint vao_;
};