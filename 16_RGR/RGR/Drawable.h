#ifndef RGR_DRAWABLE_H
#define RGR_DRAWABLE_H

#include <vector>
#include "Shader.h"

class Drawable {
private:
    Shader* shader;
    GLuint vao;
    GLuint vbo;
    GLenum drawMode;
    int verticesToDraw;
public:
    Drawable(Shader* shader, GLenum drawMode);
    void setVertices(std::vector<float> vertices);
    void draw();
};

#endif //RGR_DRAWABLE_H
