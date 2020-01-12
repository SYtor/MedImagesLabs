//
// Created by syt0r on 12/26/19.
//

#include "Point.h"

Point::Point(Shader *shader, Coordinate center, float size) : Drawable(shader, GL_POINTS) {

    std::vector<float> vertex {
            //Coordinates           //Colors
            center.x, center.y,     1.0f, 1.0f, 0.0f
    };

    Drawable::setVerticesData(vertex);

}
