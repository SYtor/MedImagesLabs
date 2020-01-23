#ifndef LAB4_IMAGE_H
#define LAB4_IMAGE_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include "Shader.h"
#include "Renderable.h"

class Image : public Renderable {
private:
    Shader *shader;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint texture;
public:
    Image(int windowWidth, int windowHeight, int imageWidth, int imageHeight, GLenum pixelType, const void* pixelData);
    ~Image();
    void render() override;
};

#endif //LAB4_IMAGE_H
