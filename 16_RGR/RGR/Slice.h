//
// Created by syt0r on 12/26/19.
//

#ifndef RGR_SLICE_H
#define RGR_SLICE_H


#include "Drawable.h"
#include "Coordinate.h"

class Slice : public Drawable {
private:

public:
    Slice(Coordinate point1, Coordinate point2, Coordinate point3, Coordinate point4);
    void draw();
};


#endif //RGR_SLICE_H
