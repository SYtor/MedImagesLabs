#include "Cube.h"
Cube::Cube(Shader* shader, float height, float width, float depth, Coordinate center) : Drawable(shader, GL_LINE_STRIP) {

    std::vector<float> vertexData = {
            //Coordinates                                                           //Colors
            center.x - width / 2, center.y + depth / 2, center.z + height / 2,      0.0f, 0.0f, 0.0f, 1.0f,
            center.x + width / 2, center.y + depth / 2, center.z + height / 2,      0.0f, 0.0f, 0.0f, 1.0f,
            center.x + width / 2, center.y - depth / 2, center.z + height / 2,      0.0f, 0.0f, 0.0f, 1.0f,
            center.x - width / 2, center.y - depth / 2, center.z + height / 2,      0.0f, 0.0f, 0.0f, 1.0f,

            center.x - width / 2, center.y + depth / 2, center.z + height / 2,      0.0f, 0.0f, 0.0f, 1.0f,
            center.x - width / 2, center.y + depth / 2, center.z - height / 2,      0.0f, 0.0f, 0.0f, 1.0f,
            center.x - width / 2, center.y - depth / 2, center.z - height / 2,      0.0f, 0.0f, 0.0f, 1.0f,
            center.x - width / 2, center.y - depth / 2, center.z + height / 2,      0.0f, 0.0f, 0.0f, 1.0f,

            center.x - width / 2, center.y - depth / 2, center.z - height / 2,      0.0f, 0.0f, 0.0f, 1.0f,
            center.x + width / 2, center.y - depth / 2, center.z - height / 2,      0.0f, 0.0f, 0.0f, 1.0f,
            center.x + width / 2, center.y - depth / 2, center.z + height / 2,      0.0f, 0.0f, 0.0f, 1.0f,

            center.x + width / 2, center.y - depth / 2, center.z - height / 2,      0.0f, 0.0f, 0.0f, 1.0f,
            center.x + width / 2, center.y + depth / 2, center.z - height / 2,      0.0f, 0.0f, 0.0f, 1.0f,
            center.x + width / 2, center.y + depth / 2, center.z + height / 2,      0.0f, 0.0f, 0.0f, 1.0f,

            center.x + width / 2, center.y + depth / 2, center.z - height / 2,      0.0f, 0.0f, 0.0f, 1.0f,
            center.x - width / 2, center.y + depth / 2, center.z - height / 2,      0.0f, 0.0f, 0.0f, 1.0f
    };

    Drawable::setVertices(vertexData);

}