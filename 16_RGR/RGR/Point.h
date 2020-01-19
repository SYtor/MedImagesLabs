#ifndef RGR_POINT_H
#define RGR_POINT_H

#include "Drawable.h"
#include "Coordinate.h"

class Point : public Drawable {
public:
    Point(Shader* shader, Coordinate center, float size);
};


#endif //RGR_POINT_H
