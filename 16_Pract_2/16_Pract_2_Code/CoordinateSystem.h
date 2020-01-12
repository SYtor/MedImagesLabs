//
// Created by syt0r on 10/10/19.
//

#ifndef LAB2_COORDINATESYSTEM_H
#define LAB2_COORDINATESYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Circle.h"
#include "Shader.h"

class CoordinateSystem {

private:
    Shader* shader;
    GLuint vao;
    GLuint vbo;

public:
    CoordinateSystem(Shader* shader, int width, int height);
    void render();

};

#endif //LAB2_COORDINATESYSTEM_H
