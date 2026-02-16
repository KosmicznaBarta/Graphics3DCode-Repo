#include "app.h"

#include <vector>

#include "spdlog/spdlog.h"

#include "glad/gl.h"

#include "Application/utils.h"

#include "glm/glm.hpp"
   
#include "glm/gtc/constants.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "Engine/camera.h"

#include "Engine/camera_controller.h"

#include "Engine/Mesh.h"

#include "Engine/KdMaterial.h"

#include "Engine/BlinnPhongMaterial.h"

#include "Engine/AbstractMaterial.h"

#include "stb/stb_image.h"

#include "Engine/texture.h"

#include "Engine/mesh_loader.h"

#include "Engine/light.h"

void SimpleShapeApplication::init() {
    // KdMaterial
    xe::KdMaterial::init();

    xe::BlinnPhongMaterial::init();

    // Camera and Controller
    set_camera(new xe::Camera);

    set_controller(new xe::CameraController(camera()));

    // PVM
    // This set up an OpenGL viewport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();

    // V_
    camera() -> look_at(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // P_
    // perspective(fov_, aspect_, near_, far_)
    camera() -> perspective(glm::radians(45.0f), double(w) / double(h), 0.1f, 20.0f);

    M_ = glm::mat4(1.0f);
    
    OGL_CALL(glCreateBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glNamedBufferData(u_trans_buffer_handle_, 2*sizeof(glm::mat4) + 12*sizeof(float), NULL, GL_DYNAMIC_DRAW));

    OGL_CALL(glCreateBuffers(2, &u_light_buffer_handle_));
    OGL_CALL(glNamedBufferData(u_light_buffer_handle_, sizeof(glm::vec3) + sizeof(int) + xe::MAX_POINT_LIGHTS*sizeof(xe::PointLight), 0, GL_DYNAMIC_DRAW));

    // PointLight(const glm::vec3 &pos, const glm::vec3 &color, float intensity, float radius) : position(pos), color(color), intensity(intensity), radius(radius) {}
    add_light(xe::PointLight(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), 1.0, 0.1));

    // Create Model from OBJReader
    auto model = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/square.obj", std::string(ROOT_DIR) + "/Models");

    add_mesh(model);

    // Setting the background color of the rendering window,
    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    OGL_CALL(glViewport(0, 0, w, h));
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    camera() -> set_aspect(double(w) / double(h));
    OGL_CALL(glViewport(0, 0, w, h));
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);   
    camera() -> zoom(yoffset / 20.0f);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);
    
    if (controller_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        controller_->LMB_pressed(x, y);
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        controller_->LMB_released(x, y);
    }
} 

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controller_) {
        controller_->mouse_moved(x, y);
    }
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    glm::mat4 PVM(1.0f);
    glm::mat4 VM(1.0f);

    // wywołanie P_
    // camera() -> projection();
    // wywołanie V_
    // camera() -> view();

    PVM = (camera() -> projection()) * (camera() -> view()) * M_;
    VM = (camera() -> view()) * M_;
    auto R = glm::mat3(VM);
    auto VM_Normal = glm::mat3(glm::cross(R[1], R[2]), glm::cross(R[2], R[0]), glm::cross(R[0], R[1]));

    OGL_CALL(glNamedBufferSubData(u_trans_buffer_handle_, 0, sizeof(glm::mat4), &PVM[0]));
    OGL_CALL(glNamedBufferSubData(u_trans_buffer_handle_, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]));
    OGL_CALL(glNamedBufferSubData(u_trans_buffer_handle_, 2*sizeof(glm::mat4), 4*sizeof(float), &VM_Normal[0]));
    OGL_CALL(glNamedBufferSubData(u_trans_buffer_handle_, 2*sizeof(glm::mat4) + 4*sizeof(float), 4*sizeof(float), &VM_Normal[1]));
    OGL_CALL(glNamedBufferSubData(u_trans_buffer_handle_, 2*sizeof(glm::mat4) + 8*sizeof(float), 4*sizeof(float), &VM_Normal[2]));

    glm::vec3 ambient(0.25f, 0.25f, 0.25f);
    int n_lights = lights_.size();

    OGL_CALL(glNamedBufferSubData(u_light_buffer_handle_, 0, sizeof(glm::vec3), &ambient));
    OGL_CALL(glNamedBufferSubData(u_light_buffer_handle_, sizeof(glm::vec3), sizeof(int), &n_lights));

    for(int i = 0; i < n_lights; i++) {
        xe::PointLight light = xe::transform(lights_[i], VM);
        OGL_CALL(glNamedBufferSubData(u_light_buffer_handle_, sizeof(glm::vec3) + sizeof(int) + i*sizeof(xe::PointLight), sizeof(xe::PointLight), &light));  
    }

    glEnable(GL_DEPTH_TEST);

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_));
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_light_buffer_handle_));

    for (auto m: meshes_) 
        m -> draw();
}