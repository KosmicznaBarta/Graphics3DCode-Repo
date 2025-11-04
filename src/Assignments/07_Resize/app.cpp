//
// Created by pbialas on 25.09.2020.
//


#include "app.h"

#include <vector>

#include "spdlog/spdlog.h"

#include "glad/gl.h"

#include "Application/utils.h"

#include "glm/glm.hpp"
   
#include "glm/gtc/constants.hpp"

#include "glm/gtc/matrix_transform.hpp"

void SimpleShapeApplication::init() {
    /*
     * A utility function that reads the shaders' source files, compiles them and creates the program object,
     * as everything in OpenGL we reference the program by an integer "handle".
     * The input to this function is a map that associates a shader type (GL_VERTEX_SHADER and GL_FRAGMENT_SHADER) with
     * its source file. The source files are located in the PROJECT_DIR/shaders directory, where  PROJECT_DIR is the
     * current assignment directory (e.g., src/Assignments/Triangle).
     */
    auto program = xe::utils::create_program(
            {
                    {GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
                    {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}
            });


    if (!program) {
        SPDLOG_CRITICAL("Invalid program");
        exit(-1);
    }

    // PVM
    // This set up an OpenGL viewport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();

    V_ = glm::lookAt(
        glm::vec3(2.0, 1.0, 2.0),
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 0.0, 1.0)
    );

    fov_ = 45.0;
    aspect_ = double(w) / double(h);
    near_ = 0.1;
    far_ = 20.0;

    M_ = glm::mat4(1.0f);
    
    OGL_CALL(glCreateBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glNamedBufferData(u_trans_buffer_handle_, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW));

    std::vector<GLubyte> index_buffer = {0, 1, 2, 3, 0, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    GLuint v_index_buffer;
    OGL_CALL(glCreateBuffers(1, &v_index_buffer));
    OGL_CALL(glNamedBufferData(v_index_buffer, index_buffer.size() * sizeof(GLubyte), index_buffer.data(), GL_STATIC_DRAW));

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
   
    /*
     * All the calls to the OpenGL API are "encapsulated" in the OGL_CALL macro for debugging purposes as explained in
     * Assignments/DEBUGGING.md. The macro is defined in src/Application/utils.h. If the call to the OpenGL API returns an
     * error code, the macro will print the name of the function that failed, the file and line number where the error
     * occurred.
     */

    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    OGL_CALL(glCreateBuffers(1, &v_buffer_handle));
    OGL_CALL(glNamedBufferData(v_buffer_handle, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW));

    // This sets up a Vertex Array Object (VAO) that encapsulates
    // the state of all vertex buffers needed for rendering.
    // The vao_ variable is a member of the SimpleShapeApplication class and is defined in src/Application/app.h.
    OGL_CALL(glGenVertexArrays(1, &vao_));
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));

    // For index_buffer between glBindVertexArray(vao_) and glBindVertexArray(0)
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_index_buffer));
    
    /*
     * The following lines bound the vertex attribute 0 to the currently bound vertex buffer (the one we just created).
     * Attribute 0 is specified in the vertex shader with the
     * layout (location = 0) in vec4 a_vertex_position;
     * directive.
     */
    // This specifies that the data for attribute 0 should be read from a vertex buffer
    OGL_CALL(glEnableVertexAttribArray(0));
    OGL_CALL(glEnableVertexAttribArray(1));
    // and this specifies the data layout in the buffer.
    OGL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                   reinterpret_cast<GLvoid *>(0)));
    OGL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                   reinterpret_cast<GLvoid *>(3*sizeof(GLfloat))));


    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    OGL_CALL(glBindVertexArray(0));
    //end of vao "recording"

    // Setting the background color of the rendering window,
    // I suggest not using white or black for better debugging.
    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    OGL_CALL(glViewport(0, 0, w, h));

    glEnable(GL_CULL_FACE);

    OGL_CALL(glUseProgram(program));
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
       Application::framebuffer_resize_callback(w, h);
       aspect_ = double(w) / double(h);
       OGL_CALL(glViewport(0, 0, w, h));
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    glm::mat4 PVM(1.0f);

    P_ = glm::perspective(
        glm::radians(fov_),
        aspect_,
        near_,
        far_
    );

    PVM = P_ * V_ * M_;

    OGL_CALL(glNamedBufferSubData(u_trans_buffer_handle_, 0, sizeof(glm::mat4), &PVM[0]));

    // Binding the VAO will set up all the required vertex attribute arrays.
    OGL_CALL(glBindVertexArray(vao_));
    glEnable(GL_DEPTH_TEST);
    OGL_CALL(glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, 0));
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_))
    OGL_CALL(glBindVertexArray(0));
}