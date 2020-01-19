#ifndef RGR_SLICE_H
#define RGR_SLICE_H

#include <GL/glew.h>
#include "Shader.h"
#include "Coordinate.h"
#include "Drawable.h"

class Slice : public Drawable {
public:
    Slice(Shader* shader, Coordinate a, Coordinate b, Coordinate c, Coordinate d);
};

#endif //RGR_SLICE_H
