//
// Created by syt0r on 12/27/19.
//

#include "Drawable.h"

Drawable::Drawable(Shader *shader, GLenum drawMode) : shader(shader), drawMode(drawMode) {}

void Drawable::setVerticesData(std::vector<float> verticesData) {

    verticesToDraw = verticesData.size() / 5;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesToDraw * sizeof(float), &verticesData[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void Drawable::draw() {
    glUseProgram(shader->getReference());
    glBindVertexArray(vao);
    glDrawArrays(drawMode, 0, verticesToDraw);
}
