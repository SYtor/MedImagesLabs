//
// Created by syt0r on 12/26/19.
//

#ifndef RGR_LINE_H
#define RGR_LINE_H

#include "Drawable.h"
#include "Coordinate.h"

class Line : public Drawable {
public:
    Line(Shader* shader, Coordinate start, Coordinate end);
};


#endif //RGR_LINE_H
