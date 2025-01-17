// Creator: Daniel Slavík
// E-Mail: sla0331@vsb.cz
// Date of Creation:  29/9/2023

//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h>

//Include GLM
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/type_ptr.hpp" // glm::value_ptr

#include "model.h"
#include <algorithm>
#include <stdexcept>

Model::Model(const float* vertices, int total_count) : vao(0), vbo(0) {

    this->vertices_count = static_cast<GLsizei>(total_count / 3 / 2);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizei>(this->vertices_count * 6 * sizeof(float)), &vertices[0],
                 GL_STATIC_DRAW);

    glGenVertexArrays(1, &this->vao); //generate the VAO
    glBindVertexArray(this->vao); //bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    // using the following lines we will tell the GPU how to read the data
    // vertex positions ->
    glEnableVertexAttribArray(0); //enable vertex attributes
    glVertexAttribPointer(0, 3,
                          GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    // vertex normals ->
    glEnableVertexAttribArray(1); //enable vertex attributes
    glVertexAttribPointer(1, 3,
                          GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

}

Model::Model(const float* vertices, int total_count, ModelOptions options) : model_options(options) {
    auto stride = getStrideFromOptions(options);
    this->vertices_count = static_cast<GLsizei>(total_count / stride);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizei>(this->vertices_count * stride * sizeof(float)), &vertices[0],
                 GL_STATIC_DRAW);

    glGenVertexArrays(1, &this->vao); //generate the VAO
    glBindVertexArray(this->vao); //bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    glEnableVertexAttribArray(0); //enable vertex attributes
    // vertex positions ->
    glVertexAttribPointer(0, 3,
                          GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride * sizeof(float)), (void*) 0);

    // using the following lines we will tell the GPU how to read the data
    // normals
    if (options & ModelOptions::NORMALS) {
        // vertex normals ->
        glEnableVertexAttribArray(1); //enable vertex attributes
        glVertexAttribPointer(1, 3,
                              GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride * sizeof(float)), (void*) (3 * sizeof(float)));
    }
    // texture coords
    if (options & ModelOptions::TEXTURED_UV) {
        // vertex texture coords ->
        glEnableVertexAttribArray(2); //enable vertex attributes
        glVertexAttribPointer(2, 2,
                              GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride * sizeof(float)), (void*) (6 * sizeof(float)));
    }
}

bool Model::isTextured() const {
    return this->model_options & ModelOptions::TEXTURED_UV || this->model_options & ModelOptions::SKYBOX;
}

Model::~Model() {
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

void Model::draw() const {
    auto draw_type = this->isStrip() ? GL_TRIANGLE_STRIP : GL_TRIANGLES;

    glBindVertexArray(this->vao);
    glDrawArrays(draw_type, 0, this->vertices_count);
}

int Model::getStrideFromOptions(ModelOptions options) {
    if (options & ModelOptions::VERTICES) {
        if (options & ModelOptions::NORMALS) {
            if (options & ModelOptions::TEXTURED_UV) {
                return 8;
            }
            return 6;
        }
        return 3;
    }
    throw std::runtime_error("Model::getStrideFromOptions: Unknown options");
}
