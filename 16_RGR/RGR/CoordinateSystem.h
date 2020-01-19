#ifndef RGR_COORDINATESYSTEM_H
#define RGR_COORDINATESYSTEM_H

#include "Drawable.h"

class CoordinateSystem : public Drawable {
public:
    CoordinateSystem(Shader* shader, float axisLength);
};


#endif //RGR_COORDINATESYSTEM_H
