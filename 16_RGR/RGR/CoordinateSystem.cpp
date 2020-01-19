#include "CoordinateSystem.h"

CoordinateSystem::CoordinateSystem(Shader *shader, float axisLength) : Drawable(shader, GL_LINES) {

    std::vector<float> vertices = {
            //X
            0,0,0, 1,0,0,1,
            axisLength,0,0, 1,0,0,1,

            0,0,0, 0,1,0,1,
            0,axisLength,0, 0,1,0,1,

            0,0,0, 0,0,1,1,
            0,0,axisLength, 0,0,1,1
    };

    Drawable::setVertices(vertices);

}