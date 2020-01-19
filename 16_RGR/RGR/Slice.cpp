#include "Slice.h"
Slice::Slice(Shader* shader, Coordinate a, Coordinate b, Coordinate c, Coordinate d) : Drawable(shader, GL_TRIANGLE_FAN) {

    std::vector<float> vertexData = {
            //Coordinates           //Colors
            a.x, a.y, a.z,          0.0f, 0.0f, 0.0f, 0.5f,
            b.x, b.y, b.z,          0.0f, 0.0f, 0.0f, 0.5f,
            c.x, c.y, c.z,          0.0f, 0.0f, 0.0f, 0.5f,
            d.x, d.y, d.z,          0.0f, 0.0f, 0.0f, 0.5f
    };

    Drawable::setVertices(vertexData);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}