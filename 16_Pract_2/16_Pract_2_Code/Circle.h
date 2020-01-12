//
// Created by syt0r on 10/10/19.
//

#ifndef LAB2_CIRCLE_H
#define LAB2_CIRCLE_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

struct CortasianCoordinates {
    float x;
    float y;
    float z;
};

struct SphericalCoordinates {
    float radialDistance;
    float theta;
    float phi;
};

class Circle {

private:
    Shader* shader;
    GLuint vao;
    GLuint vbo;

    std::vector<float> verticesData;

    void addVertex(float x, float y, float r, float g, float b);

public:

    explicit Circle(Shader* shader);

    void setCortesianParameters(CortasianCoordinates coordinates, float radius);
    void setSphericalParameters(SphericalCoordinates center, float radius);

    void render();

};

#endif //LAB2_CIRCLE_H
