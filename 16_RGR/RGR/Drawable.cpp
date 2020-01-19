#include "Drawable.h"

Drawable::Drawable(Shader *shader, GLenum drawMode) : shader(shader), drawMode(drawMode) {}

void Drawable::setVertices(std::vector<float> vertices) {

    verticesToDraw = (int) vertices.size() / 7;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glUseProgram(shader->getReference());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void Drawable::draw() {
    glUseProgram(shader->getReference());
    glBindVertexArray(vao);
    glDrawArrays(drawMode, 0, verticesToDraw);
}
