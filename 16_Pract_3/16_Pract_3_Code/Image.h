#ifndef LAB3_IMAGE_H
#define LAB3_IMAGE_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include "Shader.h"

class Image {

private:
    Shader *shader;
    GLuint vao;
    GLuint vbo;

    std::vector<float> data;

public:
    Image(int windowWidth, int windowHeight, int imageWidth, int imageHeight, const unsigned char *pixelData);
    ~Image();

    void render();

};

#endif //LAB3_IMAGE_H
