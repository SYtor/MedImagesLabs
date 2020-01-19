#include "Point.h"

Point::Point(Shader* shader, Coordinate center, float size) : Drawable(shader, GL_POINTS) {

    std::vector<float> vertexData = {
            //Coordinates                     //Colors
            center.x, center.y, center.z,     0.0f, 0.0f, 0.0f, 1.0f
    };

    Drawable::setVertices(vertexData);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(size);

}
