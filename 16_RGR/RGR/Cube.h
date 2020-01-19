//
// Created by syt0r on 1/19/20.
//

#ifndef RGR_CUBE_H
#define RGR_CUBE_H


#include <GL/glew.h>
#include "Shader.h"
#include "Coordinate.h"
#include "Drawable.h"

class Cube : public Drawable {
public:
    Cube(Shader* shader, float height, float width, float depth, Coordinate center);
};


#endif //RGR_CUBE_H
