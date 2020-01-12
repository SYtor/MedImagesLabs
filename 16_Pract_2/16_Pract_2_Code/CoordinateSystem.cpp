//
// Created by syt0r on 10/10/19.
//

#include "CoordinateSystem.h"

CoordinateSystem::CoordinateSystem(Shader* shader, int width, int height) : shader(shader) {

    int vertices[] = {
            //X
            //Coordinates   //Colors
            -width / 2, 0,  0, 0, 0,
            width / 2, 0,   0, 0, 0,
            //Y
            //Coordinates   //Colors
            0, height / 2,  0, 0, 0,
            0, -height / 2, 0, 0, 0
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 5 * sizeof(int), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_INT, GL_FALSE, 5 * sizeof(int), (void*)(2 * sizeof(int)));
    glEnableVertexAttribArray(1);

}

void CoordinateSystem::render() {
    glUseProgram(shader->getShader());
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 4);
}
