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

void SimpleShapeApplication::init() {
    auto program = xe::utils::create_program(
            {
                    {GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
                    {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}
            });


    if (!program) {
        SPDLOG_CRITICAL("Invalid program");
        exit(-1);
    }

    set_camera(new xe::Camera);

    set_controller(new xe::CameraController(camera()));

    // PVM
    // This set up an OpenGL viewport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();

    // V_
    camera() -> look_at(glm::vec3(2.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    // P_
    // perspective(fov_, aspect_, near_, far_)
    camera() -> perspective(glm::radians(45.0f), double(w) / double(h), 0.1f, 20.0f);

    M_ = glm::mat4(1.0f);
    
    OGL_CALL(glCreateBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glNamedBufferData(u_trans_buffer_handle_, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW));

    std::vector<GLubyte> index_buffer = {0, 1, 2, 3, 0, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            0.5f, -0.5f, 0.0f, 0.5, 0.5, 0.5,
            -0.5f, -0.5f, 0.0f, 0.5, 0.5, 0.5,
            -0.5f, 0.5f, 0.0f, 0.5, 0.5, 0.5,
            0.5f, 0.5f, 0.0f, 0.5, 0.5, 0.5,
            // Green
            -0.5f, 0.5f, 0.0f, 0, 1, 0,
            -0.5f, -0.5f, 0.0f, 0, 1, 0,
            0.0f, 0.0f, 1.0f, 0, 1, 0,
            // Blue
            -0.5f, 0.5f, 0.0f, 0, 0, 1,
            0.0f, 0.0f, 1.0f, 0, 0, 1,
            0.5f, 0.5f, 0.0f, 0, 0, 1,
            // Red
            0.5f, 0.5f, 0.0f, 1, 0, 0,
            0.0f, 0.0f, 1.0f, 1, 0, 0,
            0.5f, -0.5f, 0.0f, 1, 0 ,0,
            // Yellow
            0.0f, 0.0f, 1.0f, 1, 1, 0,
            -0.5f, -0.5f, 0.0f, 1, 1, 0,
            0.5f, -0.5f, 0.0f, 1, 1, 0
        };

    // Mesh
    // 1. The stride in the vertex buffer, i.e., the size of the single vertex data(attributes) in bytes.
    // 2. The size of the vertex buffer in bytes.
    // 3. The usage hint of the vertex buffer.
    // 4. The size of the index buffer in bytes.
    // 5. The type of the indexes stored in the index buffer.
    // 6. The usage hint of the index buffer.
    auto pyramid = new xe::Mesh(6 * sizeof(float), vertices.size() * sizeof(float), GL_STATIC_DRAW, 
            index_buffer.size() * sizeof(GLubyte), GL_UNSIGNED_BYTE, GL_STATIC_DRAW);

    // void load_vertices(size_t offset, size_t size, void *data);
    pyramid -> load_vertices(0, vertices.size() * sizeof(float), vertices.data());

    // void add_attribute(AttributeType attr_type, GLuint size, GLenum type, GLsizei offset) const;
    pyramid -> add_attribute(xe::AttributeType::COLOR_0, 3, GL_FLOAT, 3 * sizeof(GLfloat));

    pyramid -> add_attribute(xe::AttributeType::POSITION, 3, GL_FLOAT, 0);

    // void load_indices(size_t offset, size_t size, void *data);
    pyramid -> load_indices(0, index_buffer.size() * sizeof(GLubyte), index_buffer.data());

    pyramid -> add_primitive(0, 18);
    
    add_mesh(pyramid);

    // Setting the background color of the rendering window,
    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    OGL_CALL(glViewport(0, 0, w, h));

    glEnable(GL_CULL_FACE);

    OGL_CALL(glUseProgram(program));
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

    // wywołanie P_
    // camera() -> projection();
    // wywołanie V_
    // camera() -> view();

    PVM = (camera() -> projection()) * (camera() -> view()) * M_;

    OGL_CALL(glNamedBufferSubData(u_trans_buffer_handle_, 0, sizeof(glm::mat4), &PVM[0]));
    glEnable(GL_DEPTH_TEST);

    for (auto m: meshes_) 
        m -> draw();

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_))
}