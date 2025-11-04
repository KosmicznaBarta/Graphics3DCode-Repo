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

    GLuint v_uniform_buffer;
    OGL_CALL(glCreateBuffers(1, &v_uniform_buffer));
    OGL_CALL(glNamedBufferData(v_uniform_buffer, 8 * sizeof(float), NULL, GL_DYNAMIC_DRAW));

    float strength = 0.5;
    float mix_color[3] = {0.0, 0.0, 1.0};

    // PVM
    glm::mat4 PVM(1.0f);
    
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0, -2.0, 2.0),
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 0.0, 1.0)
    );

    // This set up an OpenGL viewport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0),
        double(w) / double(h),
        0.1,
        20.0
    );

    glm::mat4 model = glm::mat4(1.0f);

    PVM = projection * view * model; 

    PVM = glm::translate(
        PVM,
        glm::vec3(-1.0, 1.0, 0.0)
    );

    // std140 wymaga wpierw pojedynczych wartości, potem vec2, potem vec3 itp.
    OGL_CALL(glNamedBufferSubData(v_uniform_buffer, 0, sizeof(float), &strength));
    OGL_CALL(glNamedBufferSubData(v_uniform_buffer, 4 * sizeof(float), sizeof(mix_color), mix_color));

    GLuint v_uniform_pvm_buffer;
    OGL_CALL(glCreateBuffers(1, &v_uniform_pvm_buffer));
    OGL_CALL(glNamedBufferData(v_uniform_pvm_buffer, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW));

    OGL_CALL(glNamedBufferSubData(v_uniform_pvm_buffer, 0, sizeof(glm::mat4), &PVM[0]));

    // float theta = 1.0*glm::pi<float>()/6.0f; //30 degrees
    // auto cs = std::cos(theta);
    // auto ss = std::sin(theta);  
    // glm::mat2 rot{cs,ss,-ss,cs};       // cs -ss x x ss cs
    // glm::vec2 trans{0.0,  -0.25};
    // glm::vec2 scale{0.5, 0.5};

    // OGL_CALL(glNamedBufferSubData(v_uniform_move_buffer, 0, sizeof(scale), &scale));
    // OGL_CALL(glNamedBufferSubData(v_uniform_move_buffer, 2 * sizeof(float), sizeof(trans), &trans));
    // OGL_CALL(glNamedBufferSubData(v_uniform_move_buffer, 4 * sizeof(float), sizeof(glm::vec2), &rot[0]));
    // OGL_CALL(glNamedBufferSubData(v_uniform_move_buffer, 8 * sizeof(float), sizeof(glm::vec2), &rot[1]));

    std::vector<GLubyte> index_buffer = {0, 1, 2, 3, 4, 5, 4, 6, 5};
    GLuint v_index_buffer;
    OGL_CALL(glCreateBuffers(1, &v_index_buffer));
    OGL_CALL(glNamedBufferData(v_index_buffer, index_buffer.size() * sizeof(GLubyte), index_buffer.data(), GL_STATIC_DRAW));

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            -0.5f, 0.0f, 0.0f, 1, 0, 0,
            0.5f, 0.0f, 0.0f, 1, 0, 0,
            0.0f, 0.5f, 0.0f, 1, 0, 0,
            -0.5f, 0.0f, 0.0f, 0, 1, 0,
            -0.5f, -0.5f, 0.0f, 0, 1, 0,
            0.5f, 0.0f, 0.0f, 0, 1, 0,
            0.5f, -0.5f, 0.0f, 0, 1, 0,
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

    // For uniform_buffer between (vao_) and (0)
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, v_uniform_buffer));

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, v_uniform_pvm_buffer))

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

    OGL_CALL(glUseProgram(program));
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    // Binding the VAO will set up all the required vertex attribute arrays.
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_BYTE, 0));
    OGL_CALL(glBindVertexArray(0));
}