//
// Created by syt0r on 10/10/19.
//

#include <cmath>
#include <vector>
#include "Circle.h"

Circle::Circle(Shader *shader) : shader(shader) {

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

}

void Circle::setSphericalParameters(SphericalCoordinates center, float radius) {

    CortasianCoordinates cortasianCenter{};

    cortasianCenter.x = center.radialDistance * std::sin(center.theta) * std::cos(center.phi);
    cortasianCenter.y = center.radialDistance * std::sin(center.theta) * std::sin(center.phi);
    cortasianCenter.z = center.radialDistance * std::cos(center.phi);

    setCortesianParameters(cortasianCenter, radius);

}

void Circle::setCortesianParameters(CortasianCoordinates center, float radius) {

    int x = 0;
    int y = radius;
    float d = 3 - 2 * radius;

    do {
        //Create vertices
        addVertex(center.x + x, center.y + y, 0, 0, 0);
        addVertex(center.x - x, center.y - y, 0, 0, 0);
        addVertex(center.x + x, center.y - y, 0, 0, 0);
        addVertex(center.x - x, center.y + y, 0, 0, 0);

        addVertex(center.x + y, center.y + x, 0, 0, 0);
        addVertex(center.x - y, center.y - x, 0, 0, 0);
        addVertex(center.x + y, center.y - x, 0, 0, 0);
        addVertex(center.x - y, center.y + x, 0, 0, 0);

        x = x + 1;

        if(d < 0) {
            d = d + 4 * x + 6;
        }
        else {
            y = y - 1;
            d = d + 4 * (x - y) + 10;
        }

    } while (y >= x);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(float), &verticesData[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(int)));
    glEnableVertexAttribArray(1);
    
}

void Circle::addVertex(float x, float y, float r, float g, float b) {
    verticesData.push_back(x);
    verticesData.push_back(y);
    verticesData.push_back(r);
    verticesData.push_back(g);
    verticesData.push_back(b);
}

void Circle::render() {
    glUseProgram(shader->getShader());
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, verticesData.size() / 5);
}
