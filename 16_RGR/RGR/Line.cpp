//
// Created by syt0r on 12/26/19.
//

#include "Line.h"

Line::Line(Shader *shader, Coordinate start, Coordinate end) : Drawable(shader, GL_LINES) {

    std::vector<float> vertices {
            //Coordinates       //Colors
            start.x, start.y,   1.0f, 1.0f, 1.0f,
            end.x, end.y,       1.0f, 1.0f, 1.0f
    };

    Drawable::setVerticesData(vertices);

}